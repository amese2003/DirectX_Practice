#include "pch.h"
#include "20. InstancingAndCullingDemo.h"
#include "Camera.h"
#include "Waves.h"
#include "Material.h"
#include "MathHelper.h"
#include "DDSTextureLoader.h"
#include "VertexData.h"
#include "RenderObject.h"

bool InstancingAndCullingDemo::Init()
{
	if (!GameApp::Init())
		return false;

	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	_cbvSrvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_camera.SetPosition(Vec3(0.0f, 2.f, -15.f));

	LoadTextures();
	BuildRootSignature();
	BuildDescriptorHeaps();
	BuildShadersAndInputLayout();
	BuildSkullGeometry();
	BuildMaterials();
	BuildRenderItems();
	BuildFrameResources();
	BuildPSOs();

	ThrowIfFailed(_cmdList->Close());
	ID3D12CommandList* cmdsLists[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	WaitAsync();

	return true;
}

void InstancingAndCullingDemo::OnResize()
{
	GameApp::OnResize();

	_camera.SetLens(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	BoundingFrustum::CreateFromMatrix(_camFrustum, _camera.GetProj());
}

void InstancingAndCullingDemo::Update()
{
	OnKeyboardInput();

	_currFrameResourceIndex = (_currFrameResourceIndex + 1) % gNumFrameResources;
	_currFrameResource = _frameResources[_currFrameResourceIndex];


	if (_currFrameResource->Fence != 0 && _fence->GetCompletedValue() < _currFrameResource->Fence)
	{
		HANDLE eventHandle = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
		CHECK(_fence->SetEventOnCompletion(_currFrameResource->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	AnimateMaterials();
	UpdateInstanceData();
	UpdateMaterialBuffer();
	UpdateMainPassCB();
}

void InstancingAndCullingDemo::Draw()
{
	auto cmdListAlloc = _currFrameResource->CmdListAlloc;

	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(cmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(_cmdList->Reset(cmdListAlloc.Get(), _pipelineStates["opaque"].Get()));

	_cmdList->RSSetViewports(1, &_screenViewport);
	_cmdList->RSSetScissorRects(1, &_scissorRect);

	// Indicate a state transition on the resource usage.
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(GetBackBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	_cmdList->ResourceBarrier(1, &barrier);

	// Clear the back buffer and depth buffer.
	_cmdList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	_cmdList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	auto currentBackBufferView = CurrentBackBufferView();
	auto depthStencil = DepthStencilView();

	_cmdList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencil);

	ID3D12DescriptorHeap* descriptorHeaps[] = { _srvHeap.Get() };
	_cmdList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	_cmdList->SetGraphicsRootSignature(_rootSignature.Get());

	// Bind all the materials used in this scene.  For structured buffers, we can bypass the heap and 
	// set as a root descriptor.
	auto matBuffer = _currFrameResource->MaterialBuffer->Resource();
	_cmdList->SetGraphicsRootShaderResourceView(1, matBuffer->GetGPUVirtualAddress());

	auto passCB = _currFrameResource->PassCB->Resource();
	_cmdList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

	// Bind all the textures used in this scene.
	_cmdList->SetGraphicsRootDescriptorTable(3, _srvHeap->GetGPUDescriptorHandleForHeapStart());

	DrawRenderItems(_cmdList.Get(), _renderItems);

	// Indicate a state transition on the resource usage.
	barrier = CD3DX12_RESOURCE_BARRIER::Transition(GetBackBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	_cmdList->ResourceBarrier(1, &barrier);

	// Done recording commands.
	ThrowIfFailed(_cmdList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Swap the back and front buffers
	ThrowIfFailed(_swapChain->Present(0, 0));
	_curBackbuffer = (_curBackbuffer + 1) % _swapChainCount;

	// Advance the fence value to mark commands up to this fence point.
	_currFrameResource->Fence = ++_currentFence;

	// Add an instruction to the command queue to set a new fence point. 
	// Because we are on the GPU timeline, the new fence point won't be 
	// set until the GPU finishes processing all the commands prior to this Signal().
	_cmdQueue->Signal(_fence.Get(), _currentFence);
}

void InstancingAndCullingDemo::OnMouseDown(WPARAM btnState, int x, int y)
{
	_lastMousepos.x = x;
	_lastMousepos.y = y;

	SetCapture(_mainWnd);
}

void InstancingAndCullingDemo::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void InstancingAndCullingDemo::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - _lastMousepos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - _lastMousepos.y));

		_camera.Pitch(dy);
		_camera.RotateY(dx);
	}

	_lastMousepos.x = x;
	_lastMousepos.y = y;
}

void InstancingAndCullingDemo::OnKeyboardInput()
{
	const float dt = TIME->DeltaTime();

	if (GetAsyncKeyState('W') & 0x8000)
		_camera.Walk(20.0f * dt);

	if (GetAsyncKeyState('S') & 0x8000)
		_camera.Walk(-20.0f * dt);

	if (GetAsyncKeyState('A') & 0x8000)
		_camera.Strafe(-20.0f * dt);

	if (GetAsyncKeyState('D') & 0x8000)
		_camera.Strafe(20.0f * dt);

	if (GetAsyncKeyState('1') & 0x8000)
		_frustumEnable = true;

	if (GetAsyncKeyState('2') & 0x8000)
		_frustumEnable = false;

	_camera.UpdateViewMatrix();
}

void InstancingAndCullingDemo::AnimateMaterials()
{
}

void InstancingAndCullingDemo::UpdateInstanceData()
{
	XMMATRIX view = _camera.GetView();

	auto determin = XMMatrixDeterminant(view);
	XMMATRIX invView = XMMatrixInverse(&determin, view);

	auto currInstanceBuffer = _currFrameResource->InstanceBuffer.get();
	for (auto& e : _gameObjects)
	{
		const auto& instanceData = e->GetInstanceData();

		int visibleInstanceCount = 0;

		for (UINT i = 0; i < (UINT)instanceData.size(); ++i)
		{
			XMMATRIX world = XMLoadFloat4x4(&instanceData[i].World);
			XMMATRIX texTransform = XMLoadFloat4x4(&instanceData[i].TexTransform);

			auto worldDetermin = XMMatrixDeterminant(world);
			XMMATRIX invWorld = XMMatrixInverse(&worldDetermin, world);

			// View space to the object's local space.
			XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);

			// Transform the camera frustum from view space to the object's local space.
			BoundingFrustum localSpaceFrustum;
			_camFrustum.Transform(localSpaceFrustum, viewToLocal);

			// Perform the box/frustum intersection test in local space.
			if ((localSpaceFrustum.Contains(e->GetBoundingBox()) != DirectX::DISJOINT) || (_frustumEnable == false))
			{
				InstanceData data;
				XMStoreFloat4x4(&data.World, XMMatrixTranspose(world));
				XMStoreFloat4x4(&data.TexTransform, XMMatrixTranspose(texTransform));
				data.MaterialIndex = instanceData[i].MaterialIndex;

				// Write the instance data to structured buffer for the visible objects.
				currInstanceBuffer->CopyData(visibleInstanceCount++, data);
			}
		}

		e->SetInstanceCount(visibleInstanceCount);

		std::wostringstream outs;
		outs.precision(6);
		outs << L"Instancing and Culling Demo" <<
			L"    " << e->GetInstanceCount() <<
			L" objects visible out of " << e->GetInstanceData().size();
		_mainCaption = outs.str();
	}
}

void InstancingAndCullingDemo::UpdateMaterialBuffer()
{
	auto currMaterialBuffer = _currFrameResource->MaterialBuffer.get();
	for (auto& e : _material)
	{
		// Only update the cbuffer data if the constants have changed.  If the cbuffer
		// data changes, it needs to be updated for each FrameResource.
		shared_ptr<Material> mat = e.second;
		if (mat->NumFramesDirty > 0)
		{
			XMMATRIX matTransform = XMLoadFloat4x4(&mat->MatTransform);

			MaterialData matData;
			matData.DiffuseAlbedo = mat->DiffuseAlbedo;
			matData.FresnelR0 = mat->FresnelR0;
			matData.Roughness = mat->Roughness;
			XMStoreFloat4x4(&matData.MatTransform, XMMatrixTranspose(matTransform));
			matData.DiffuseMapIndex = mat->DiffuseSrvHeapIndex;

			currMaterialBuffer->CopyData(mat->MatCBIndex, matData);

			// Next FrameResource need to be updated too.
			mat->NumFramesDirty--;
		}
	}
}

void InstancingAndCullingDemo::UpdateMainPassCB()
{
	XMMATRIX view = _camera.GetView();
	XMMATRIX proj = _camera.GetProj();

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);

	auto viewdterminant = XMMatrixDeterminant(view);
	XMMATRIX invView = XMMatrixInverse(&viewdterminant, view);
	auto projdeterminant = XMMatrixDeterminant(proj);
	XMMATRIX invProj = XMMatrixInverse(&projdeterminant, proj);
	auto viewprojDeterminat = XMMatrixDeterminant(viewProj);
	XMMATRIX invViewProj = XMMatrixInverse(&viewprojDeterminat, viewProj);

	XMStoreFloat4x4(&_mainConstant.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&_mainConstant.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&_mainConstant.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&_mainConstant.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&_mainConstant.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&_mainConstant.InvViewProj, XMMatrixTranspose(invViewProj));
	_mainConstant.EyePosW = _camera.GetPosition3f();
	_mainConstant.RenderTargetSize = XMFLOAT2((float)_clientWidth, (float)_clientHeight);
	_mainConstant.InvRenderTargetSize = XMFLOAT2(1.0f / _clientWidth, 1.0f / _clientHeight);
	_mainConstant.NearZ = 1.0f;
	_mainConstant.FarZ = 1000.0f;
	_mainConstant.TotalTime = TIME->TotalTime();
	_mainConstant.DeltaTime = TIME->DeltaTime();
	_mainConstant.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	_mainConstant.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	_mainConstant.Lights[0].Strength = { 0.8f, 0.8f, 0.8f };
	_mainConstant.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	_mainConstant.Lights[1].Strength = { 0.4f, 0.4f, 0.4f };
	_mainConstant.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	_mainConstant.Lights[2].Strength = { 0.2f, 0.2f, 0.2f };

	auto currPassCB = _currFrameResource->PassCB.get();
	currPassCB->CopyData(0, _mainConstant);
}


InstancingAndCullingDemo::InstancingAndCullingDemo(HINSTANCE hInstance) : GameApp(hInstance)
{
}

InstancingAndCullingDemo::~InstancingAndCullingDemo()
{
}

void InstancingAndCullingDemo::LoadTextures()
{
	auto bricksTex = make_shared<Texture>();
	bricksTex->Name = "bricksTex";
	bricksTex->Filename = L"../Resources/Textures/bricks.dds";
	CHECK(::CreateDDSTextureFromFile12(_device.Get(), _cmdList.Get(), bricksTex->Filename.c_str(), bricksTex->Resource, bricksTex->UploadHeap));

	auto stoneTex = make_shared<Texture>();
	stoneTex->Name = "stoneTex";
	stoneTex->Filename = L"../Resources/Textures/stone.dds";
	CHECK(::CreateDDSTextureFromFile12(_device.Get(), _cmdList.Get(), stoneTex->Filename.c_str(), stoneTex->Resource, stoneTex->UploadHeap));

	auto tileTex = make_shared<Texture>();
	tileTex->Name = "tileTex";
	tileTex->Filename = L"../Resources/Textures/tile.dds";
	CHECK(::CreateDDSTextureFromFile12(_device.Get(), _cmdList.Get(), tileTex->Filename.c_str(), tileTex->Resource, tileTex->UploadHeap));

	auto crateTex = make_shared<Texture>();
	crateTex->Name = "crateTex";
	crateTex->Filename = L"../Resources/Textures/WoodCrate01.dds";
	CHECK(::CreateDDSTextureFromFile12(_device.Get(), _cmdList.Get(), crateTex->Filename.c_str(), crateTex->Resource, crateTex->UploadHeap));

	auto iceTex = make_shared<Texture>();
	iceTex->Name = "iceTex";
	iceTex->Filename = L"../Resources/Textures/ice.dds";
	CHECK(::CreateDDSTextureFromFile12(_device.Get(), _cmdList.Get(), iceTex->Filename.c_str(), iceTex->Resource, iceTex->UploadHeap));

	auto grassTex = make_shared<Texture>();
	grassTex->Name = "grassTex";
	grassTex->Filename = L"../Resources/Textures/grass.dds";
	CHECK(::CreateDDSTextureFromFile12(_device.Get(), _cmdList.Get(), grassTex->Filename.c_str(), grassTex->Resource, grassTex->UploadHeap));

	auto defaultTex = make_shared<Texture>();
	defaultTex->Name = "defaultTex";
	defaultTex->Filename = L"../Resources/Textures/white1x1.dds";
	CHECK(::CreateDDSTextureFromFile12(_device.Get(), _cmdList.Get(), defaultTex->Filename.c_str(), defaultTex->Resource, defaultTex->UploadHeap));

	// R°ª?
	_textures[bricksTex->Name] = ::move(bricksTex);
	_textures[stoneTex->Name] = ::move(stoneTex);
	_textures[tileTex->Name] = ::move(tileTex);
	_textures[crateTex->Name] = ::move(crateTex);
	_textures[iceTex->Name] = ::move(iceTex);
	_textures[grassTex->Name] = ::move(grassTex);
	_textures[defaultTex->Name] = ::move(defaultTex);
}

void InstancingAndCullingDemo::BuildRootSignature()
{

	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 7, 0);

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER params[4];

	params[0].InitAsShaderResourceView(0, 1);
	params[1].InitAsShaderResourceView(1, 1);
	params[2].InitAsConstantBufferView(0);
	params[3].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	

	auto staticSamplers = GetStaticSamplers();

	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(params), params, (UINT)staticSamplers.size(), staticSamplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	HRESULT hr = ::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);

	if (blobError != nullptr)
	{
		::OutputDebugStringA((char*)blobError->GetBufferPointer());
	}
	CHECK(hr);

	hr = _device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
	CHECK(hr);
}

void InstancingAndCullingDemo::BuildDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 7;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap)));

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(_srvHeap->GetCPUDescriptorHandleForHeapStart());

	auto bricksTex = _textures["bricksTex"]->Resource;
	auto stoneTex = _textures["stoneTex"]->Resource;
	auto tileTex = _textures["tileTex"]->Resource;
	auto crateTex = _textures["crateTex"]->Resource;
	auto iceTex = _textures["iceTex"]->Resource;
	auto grassTex = _textures["grassTex"]->Resource;
	auto defaultTex = _textures["defaultTex"]->Resource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = bricksTex->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = bricksTex->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	_device->CreateShaderResourceView(bricksTex.Get(), &srvDesc, hDescriptor);


	hDescriptor.Offset(1, _cbvSrvDescriptorSize);

	srvDesc.Format = stoneTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = stoneTex->GetDesc().MipLevels;
	_device->CreateShaderResourceView(stoneTex.Get(), &srvDesc, hDescriptor);


	hDescriptor.Offset(1, _cbvSrvDescriptorSize);

	srvDesc.Format = tileTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = tileTex->GetDesc().MipLevels;
	_device->CreateShaderResourceView(tileTex.Get(), &srvDesc, hDescriptor);


	hDescriptor.Offset(1, _cbvSrvDescriptorSize);

	srvDesc.Format = crateTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = crateTex->GetDesc().MipLevels;
	_device->CreateShaderResourceView(crateTex.Get(), &srvDesc, hDescriptor);


	hDescriptor.Offset(1, _cbvSrvDescriptorSize);

	srvDesc.Format = iceTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = iceTex->GetDesc().MipLevels;
	_device->CreateShaderResourceView(iceTex.Get(), &srvDesc, hDescriptor);


	hDescriptor.Offset(1, _cbvSrvDescriptorSize);

	srvDesc.Format = grassTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = grassTex->GetDesc().MipLevels;
	_device->CreateShaderResourceView(grassTex.Get(), &srvDesc, hDescriptor);


	hDescriptor.Offset(1, _cbvSrvDescriptorSize);

	srvDesc.Format = defaultTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = defaultTex->GetDesc().MipLevels;
	_device->CreateShaderResourceView(defaultTex.Get(), &srvDesc, hDescriptor);
}

void InstancingAndCullingDemo::BuildShadersAndInputLayout()
{
	const D3D_SHADER_MACRO alphaTestDefines[] =
	{
		"ALPHA_TEST", "1",
		NULL, NULL
	};

	_shaders["defaultVS"] = d3dUtil::CompileShader(L"..\\Shaders\\19. InstancedBasic.fx", nullptr, "VS", "vs_5_1");
	_shaders["defaultPS"] = d3dUtil::CompileShader(L"..\\Shaders\\19. InstancedBasic.fx", nullptr, "PS", "ps_5_1");

	_inputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
}

void InstancingAndCullingDemo::BuildSkullGeometry()
{
	std::ifstream fin("../Resources/Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, L"Models/skull.txt not found.", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);


	vector<VertexTextureNormalData> vertices;
	vertices.resize(vcount);

	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;
		fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;

		XMVECTOR P = XMLoadFloat3(&vertices[i].position);

		// Project point onto unit sphere and generate spherical texture coordinates.
		XMFLOAT3 spherePos;
		XMStoreFloat3(&spherePos, XMVector3Normalize(P));

		float theta = atan2f(spherePos.z, spherePos.x);

		// Put in [0, 2pi].
		if (theta < 0.0f)
			theta += XM_2PI;

		float phi = acosf(spherePos.y);

		float u = theta / (2.0f * XM_PI);
		float v = phi / XM_PI;

		vertices[i].uv = { u, v };

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	BoundingBox bounds;
	XMStoreFloat3(&bounds.Center, 0.5f * (vMin + vMax));
	XMStoreFloat3(&bounds.Extents, 0.5f * (vMax - vMin));

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	vector<int32> indices(3 * tcount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	const UINT vertexBufferSize = (UINT)vertices.size() * sizeof(VertexTextureNormalData);
	const UINT indexBufferSize = (UINT)indices.size() * sizeof(int32);

	fin.close();

	auto geometry = make_shared<MeshGeometry>();
	geometry->Name = "skullGeo";

	CHECK(D3DCreateBlob(vertexBufferSize, &geometry->VertexBufferCPU));
	CopyMemory(geometry->VertexBufferCPU->GetBufferPointer(), vertices.data(), vertexBufferSize);


	CHECK(D3DCreateBlob(indexBufferSize, &geometry->IndexBufferCPU));
	CopyMemory(geometry->IndexBufferCPU->GetBufferPointer(), indices.data(), indexBufferSize);

	geometry->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(_device.Get(),
		_cmdList.Get(), vertices.data(), vertexBufferSize, geometry->VertexBufferUploader);

	geometry->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(_device.Get(),
		_cmdList.Get(), indices.data(), indexBufferSize, geometry->IndexBufferUploader);

	geometry->VertexByteStride = sizeof(VertexTextureNormalData);
	geometry->VertexBufferByteSize = vertexBufferSize;
	geometry->IndexFormat = DXGI_FORMAT_R32_UINT;
	geometry->IndexBufferByteSize = indexBufferSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	submesh.Bounds = bounds;

	geometry->DrawArgs["skull"] = submesh;
	_geometry[geometry->Name] = ::move(geometry);

}



void InstancingAndCullingDemo::BuildMaterials()
{
	auto bricks0 = make_shared<Material>();
	bricks0->Name = "bricks0";
	bricks0->MatCBIndex = 0;
	bricks0->DiffuseSrvHeapIndex = 0;
	bricks0->DiffuseAlbedo = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	bricks0->FresnelR0 = Vec3(0.02f, 0.02f, 0.02f);
	bricks0->Roughness = 0.1f;

	auto stone0 = make_shared<Material>();
	stone0->Name = "stone0";
	stone0->MatCBIndex = 1;
	stone0->DiffuseSrvHeapIndex = 1;
	stone0->DiffuseAlbedo = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	stone0->FresnelR0 = Vec3(0.05f, 0.05f, 0.05f);
	stone0->Roughness = 0.3f;

	auto tile0 = make_shared<Material>();
	tile0->Name = "tile0";
	tile0->MatCBIndex = 2;
	tile0->DiffuseSrvHeapIndex = 2;
	tile0->DiffuseAlbedo = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	tile0->FresnelR0 = Vec3(0.02f, 0.02f, 0.02f);
	tile0->Roughness = 0.3f;

	auto crate0 = make_shared<Material>();
	crate0->Name = "checkboard0";
	crate0->MatCBIndex = 3;
	crate0->DiffuseSrvHeapIndex = 3;
	crate0->DiffuseAlbedo = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	crate0->FresnelR0 = Vec3(0.05f, 0.05f, 0.05f);
	crate0->Roughness = 0.2f;

	auto ice0 = make_shared<Material>();
	ice0->Name = "ice0";
	ice0->MatCBIndex = 4;
	ice0->DiffuseSrvHeapIndex = 4;
	ice0->DiffuseAlbedo = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ice0->FresnelR0 = Vec3(0.1f, 0.1f, 0.1f);
	ice0->Roughness = 0.0f;

	auto grass0 = make_shared<Material>();
	grass0->Name = "grass0";
	grass0->MatCBIndex = 5;
	grass0->DiffuseSrvHeapIndex = 5;
	grass0->DiffuseAlbedo = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	grass0->FresnelR0 = Vec3(0.05f, 0.05f, 0.05f);
	grass0->Roughness = 0.2f;

	auto skullMat = make_shared<Material>();
	skullMat->Name = "skullMat";
	skullMat->MatCBIndex = 6;
	skullMat->DiffuseSrvHeapIndex = 6;
	skullMat->DiffuseAlbedo = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	skullMat->FresnelR0 = Vec3(0.05f, 0.05f, 0.05f);
	skullMat->Roughness = 0.5f;

	_material["bricks0"] = ::move(bricks0);
	_material["stone0"] = ::move(stone0);
	_material["tile0"] = ::move(tile0);
	_material["checkboard0"] = ::move(crate0);
	_material["ice0"] = ::move(ice0);
	_material["grass0"] = ::move(grass0);
	_material["skullMat"] = ::move(skullMat);
}

void InstancingAndCullingDemo::BuildRenderItems()
{
	auto skullObject = make_shared<RenderObject>();
	skullObject->SetWorldMatrix(MathHelper::Identity4x4());
	skullObject->SetTexMatrix(MathHelper::Identity4x4());
	skullObject->SetConstantIndex(0);
	skullObject->SetMaterial(_material["tile0"]);

	auto targetGeo = _geometry["skullGeo"];

	skullObject->SetGeometry(targetGeo);
	skullObject->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	skullObject->SetIndexCount(targetGeo->DrawArgs["skull"].IndexCount);
	skullObject->SetStartIndexLocation(targetGeo->DrawArgs["skull"].StartIndexLocation);
	skullObject->SetBaseIndexLocation(targetGeo->DrawArgs["skull"].BaseVertexLocation);
	skullObject->SetBoundingBox(targetGeo->DrawArgs["skull"].Bounds);

	const int n = 5;
	_instanceCount = n * n * n;
	skullObject->SetInstanceCount(n * n * n);
	skullObject->SetInstanceSize(n * n * n);

	float width = 200.0f;
	float height = 200.0f;
	float depth = 200.0f;

	float x = -0.5f * width;
	float y = -0.5f * height;
	float z = -0.5f * depth;
	float dx = width / (n - 1);
	float dy = height / (n - 1);
	float dz = depth / (n - 1);

	for (int k = 0; k < n; ++k)
	{
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				int index = k * n * n + i * n + j;
				// Position instanced along a 3D grid.

				InstanceData data;

				data.World = XMFLOAT4X4(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					x + j * dx, y + i * dy, z + k * dz, 1.0f);

				data.TexTransform = Matrix::CreateScale(Vec3(2.0f, 2.0f, 1.0f));
				data.MaterialIndex = index % _material.size();

				skullObject->SetInstanceData(index, data);
			}
		}
	}

	_gameObjects.push_back(::move(skullObject));

	for (auto& e : _gameObjects)
	{
		_renderItems.push_back(e);
	}
}



void InstancingAndCullingDemo::BuildFrameResources()
{
	for (int i = 0; i < gNumFrameResources; i++)
	{
		shared_ptr<InstanceResource> resources = make_shared<InstanceResource>(_device.Get(), 1, _instanceCount, (UINT)_material.size());
		_frameResources.push_back(resources);
	}
}

void InstancingAndCullingDemo::BuildPSOs()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStates;

	ZeroMemory(&pipelineStates, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	pipelineStates.InputLayout = { _inputLayout.data(), (UINT)_inputLayout.size() };
	pipelineStates.pRootSignature = _rootSignature.Get();
	pipelineStates.VS = { reinterpret_cast<BYTE*>(_shaders["defaultVS"]->GetBufferPointer()), _shaders["defaultVS"]->GetBufferSize() };
	pipelineStates.PS = { reinterpret_cast<BYTE*>(_shaders["defaultPS"]->GetBufferPointer()), _shaders["defaultPS"]->GetBufferSize() };
	pipelineStates.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineStates.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	pipelineStates.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipelineStates.SampleMask = UINT_MAX;
	pipelineStates.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineStates.NumRenderTargets = 1;
	pipelineStates.RTVFormats[0] = _backBufferFormat;
	pipelineStates.SampleDesc.Count = _4xMsaaState ? 4 : 1;
	pipelineStates.SampleDesc.Quality = _4xMsaaState ? (_4xMassQuality - 1) : 0;
	pipelineStates.DSVFormat = _depthStencilFormat;
	ThrowIfFailed(_device->CreateGraphicsPipelineState(&pipelineStates, IID_PPV_ARGS(&_pipelineStates["opaque"])));
}

void InstancingAndCullingDemo::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<shared_ptr<RenderObject>>& ritems)
{
	// For each render item...
	for (size_t i = 0; i < ritems.size(); ++i)
	{
		auto ri = ritems[i];

		shared_ptr<MeshGeometry> geo = ri->GetGeometry();


		auto vertexView = geo->VertexBufferView();
		auto indexView = geo->IndexBufferView();
		cmdList->IASetVertexBuffers(0, 1, &vertexView);
		cmdList->IASetIndexBuffer(&indexView);
		cmdList->IASetPrimitiveTopology(ri->GetPrimitiveType());

		// Set the instance buffer to use for this render-item.  For structured buffers, we can bypass 
		// the heap and set as a root descriptor.
		auto instanceBuffer = _currFrameResource->InstanceBuffer->Resource();
		_cmdList->SetGraphicsRootShaderResourceView(0, instanceBuffer->GetGPUVirtualAddress());

		cmdList->DrawIndexedInstanced(ri->GetIndexCount(), ri->GetInstanceCount(), ri->GetStartIndexLocation(), ri->GetBaseVertexLocation(), 0);
	}
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> InstancingAndCullingDemo::GetStaticSamplers()
{
	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}
