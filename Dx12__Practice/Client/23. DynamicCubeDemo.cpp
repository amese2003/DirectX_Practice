#include "pch.h"
#include "23. DynamicCubeDemo.h"
#include "Camera.h"
#include "Waves.h"
#include "Material.h"
#include "MathHelper.h"
#include "DDSTextureLoader.h"
#include "VertexData.h"
#include "RenderObject.h"
#include "GeometryGenerator.h"

bool DynamicCubeDemo::Init()
{
	if (!GameApp::Init())
		return false;

	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	_cbvSrvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	_camera.SetPosition(0.f, 2.f, -15.f);
	_camera.LookAt(
		Vec3(5.f, 4.f, -15.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 1.f, 0.f)
	);

	BuildCubeFaceCamera(0.0f, 2.0f, 0.0f);


	_dynamicCubeMap = make_shared<CubeRenderTarget>(_device, CubeMapSize, CubeMapSize, DXGI_FORMAT_R8G8B8A8_UNORM);

	LoadTextures();
	BuildRootSignature();
	BuildDescriptorHeaps();
	BuildCubeDepthStencil();
	BuildShadersAndInputLayout();
	BuildMeshGeometry();
	BuildCarGeometry();
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

	std::wostringstream outs;
	outs.precision(6);
	outs << L"Dynamic CubeMap Demo ";
	_mainCaption = outs.str();

	return true;
}

void DynamicCubeDemo::CreateRtvAndDsvDescriptorHeaps()
{
	// Add +6 RTV for cube render target.
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = _swapChainCount + 6;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	ThrowIfFailed(_device->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(_rtvHeap.GetAddressOf())));

	// Add +1 DSV for cube render target.
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 2;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	ThrowIfFailed(_device->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(_dsvHeap.GetAddressOf())));

	_cubedsvhandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_dsvHeap->GetCPUDescriptorHandleForHeapStart(), 1, _dsvDescriptorSize);
}

void DynamicCubeDemo::OnResize()
{
	GameApp::OnResize();

	_camera.SetLens(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	//BoundingFrustum::CreateFromMatrix(_camFrustum, _camera.GetProj());
}

void DynamicCubeDemo::Update()
{
	OnKeyboardInput();

	XMMATRIX skullScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	XMMATRIX skullOffset = XMMatrixTranslation(3.0f, 2.0f, 0.0f);
	XMMATRIX skullLocalRotate = XMMatrixRotationY(2.0f * TIME->TotalTime());
	XMMATRIX skullGlobalRotate = XMMatrixRotationY(0.5f * TIME->TotalTime());
	_skullItem->SetWorldMatrix(skullScale * skullLocalRotate * skullOffset * skullGlobalRotate);
	_skullItem->SetDirty(gNumFrameResources);



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
	UpdateObjectConstant();
	UpdateMaterialBuffer();
	UpdateMainPassCB();
}

void DynamicCubeDemo::Draw()
{
	auto cmdListAlloc = _currFrameResource->CmdListAlloc;

	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(cmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(_cmdList->Reset(cmdListAlloc.Get(), _pipelineStates["opaque"].Get()));

	ID3D12DescriptorHeap* descriptorHeaps[] = { _srvHeap.Get() };
	_cmdList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	_cmdList->SetGraphicsRootSignature(_rootSignature.Get());


	auto matBuffer = _currFrameResource->MaterialBuffer->Resource();
	_cmdList->SetGraphicsRootShaderResourceView(2, matBuffer->GetGPUVirtualAddress());

	CD3DX12_GPU_DESCRIPTOR_HANDLE skyTexDescriptor(_srvHeap->GetGPUDescriptorHandleForHeapStart());
	skyTexDescriptor.Offset(_skyTexHeapIndex, _cbvSrvDescriptorSize);;
	_cmdList->SetGraphicsRootDescriptorTable(3, skyTexDescriptor);

	// Bind all the textures used in this scene.
	_cmdList->SetGraphicsRootDescriptorTable(4, _srvHeap->GetGPUDescriptorHandleForHeapStart());


	DrawSceneToCubeMap();

	_cmdList->RSSetViewports(1, &_screenViewport);
	_cmdList->RSSetScissorRects(1, &_scissorRect);

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

	auto passCB = _currFrameResource->PassCB->Resource();
	_cmdList->SetGraphicsRootConstantBufferView(0, passCB->GetGPUVirtualAddress());


	// Use the dynamic cube map for the dynamic reflectors layer.
	CD3DX12_GPU_DESCRIPTOR_HANDLE dynamicTexDescriptor(_srvHeap->GetGPUDescriptorHandleForHeapStart());
	dynamicTexDescriptor.Offset(_skyTexHeapIndex + 1, _cbvSrvUavDescriptorSize);
	_cmdList->SetGraphicsRootDescriptorTable(3, dynamicTexDescriptor);

	DrawRenderItems(_cmdList.Get(), _renderItems[static_cast<int>(RenderLayer::OpaqueDynamicReflectors)]);


	_cmdList->SetGraphicsRootDescriptorTable(3, skyTexDescriptor);


	DrawRenderItems(_cmdList.Get(), _renderItems[static_cast<int>(RenderLayer::Opaque)]);


	_cmdList->SetPipelineState(_pipelineStates["sky"].Get());
	DrawRenderItems(_cmdList.Get(), _renderItems[static_cast<int>(RenderLayer::Sky)]);


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

void DynamicCubeDemo::OnMouseDown(WPARAM btnState, int x, int y)
{
	_lastMousepos.x = x;
	_lastMousepos.y = y;

	SetCapture(_mainWnd);
}

void DynamicCubeDemo::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void DynamicCubeDemo::OnMouseMove(WPARAM btnState, int x, int y)
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

void DynamicCubeDemo::OnKeyboardInput()
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

	//if (GetAsyncKeyState('1') & 0x8000)
	//	_frustumEnable = true;

	//if (GetAsyncKeyState('2') & 0x8000)
	//	_frustumEnable = false;

	_camera.UpdateViewMatrix();
}

void DynamicCubeDemo::AnimateMaterials()
{
}

void DynamicCubeDemo::UpdateObjectConstant()
{
	auto currObjectConstant = _currFrameResource->ObjectCB;

	for (shared_ptr<RenderObject> gameObject : _gameObjects)
	{
		int dirty = gameObject->GetnumframedIrty();

		if (dirty > 0)
		{
			XMMATRIX world = gameObject->GetWorldMatrix();
			XMMATRIX tex = gameObject->GetTexMatrix();
			

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(tex));
			objConstants.MaterialIndex = gameObject->GetMaterial()->MatCBIndex;

			currObjectConstant->CopyData(gameObject->GetConstantIndex(), objConstants);
			gameObject->SetDirty(dirty - 1);
		}
	}
}



void DynamicCubeDemo::UpdateMaterialBuffer()
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

void DynamicCubeDemo::UpdateMainPassCB()
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

	UpdateCubeMapConstant();
}

void DynamicCubeDemo::UpdateCubeMapConstant()
{
	for (int i = 0; i < 6; i++)
	{
		PassConstants cubeFacePassCB = _mainConstant;

		XMMATRIX view = _cubeMapCamera[i].GetView();
		XMMATRIX proj = _cubeMapCamera[i].GetProj();

		XMMATRIX viewProj = XMMatrixMultiply(view, proj);

		auto viewdterminant = XMMatrixDeterminant(view);
		XMMATRIX invView = XMMatrixInverse(&viewdterminant, view);
		auto projdeterminant = XMMatrixDeterminant(proj);
		XMMATRIX invProj = XMMatrixInverse(&projdeterminant, proj);
		auto viewprojDeterminat = XMMatrixDeterminant(viewProj);
		XMMATRIX invViewProj = XMMatrixInverse(&viewprojDeterminat, viewProj);



		XMStoreFloat4x4(&cubeFacePassCB.View, XMMatrixTranspose(view));
		XMStoreFloat4x4(&cubeFacePassCB.InvView, XMMatrixTranspose(invView));
		XMStoreFloat4x4(&cubeFacePassCB.Proj, XMMatrixTranspose(proj));
		XMStoreFloat4x4(&cubeFacePassCB.InvProj, XMMatrixTranspose(invProj));
		XMStoreFloat4x4(&cubeFacePassCB.ViewProj, XMMatrixTranspose(viewProj));
		XMStoreFloat4x4(&cubeFacePassCB.InvViewProj, XMMatrixTranspose(invViewProj));

		cubeFacePassCB.EyePosW = _cubeMapCamera[i].GetPosition3f();
		cubeFacePassCB.RenderTargetSize = XMFLOAT2((float)CubeMapSize, (float)CubeMapSize);
		cubeFacePassCB.InvRenderTargetSize = XMFLOAT2(1.0f / CubeMapSize, 1.0f / CubeMapSize);

		auto currPassCB = _currFrameResource->PassCB.get();

		// Cube map pass cbuffers are stored in elements 1-6.
		currPassCB->CopyData(1 + i, cubeFacePassCB);
	}
}


DynamicCubeDemo::DynamicCubeDemo(HINSTANCE hInstance) : GameApp(hInstance)
{
}

DynamicCubeDemo::~DynamicCubeDemo()
{
	if (_device != nullptr)
		WaitAsync();
}

void DynamicCubeDemo::LoadTextures()
{
	vector<string> texName =
	{
		"bricksTex",
		"stoneTex",
		"tileTex",
		"crateTex",
		"iceTex",
		"grassTex",
		"defaultTex",
		"skyCube"
	};

	vector<wstring> fileName =
	{
		L"../Resources/Textures/bricks2.dds",
		L"../Resources/Textures/stone.dds",
		L"../Resources/Textures/tile.dds",
		L"../Resources/Textures/WoodCrate01.dds",
		L"../Resources/Textures/ice.dds",
		L"../Resources/Textures/grass.dds",
		L"../Resources/Textures/white1x1.dds",
		L"../Resources/Textures/grasscube1024.dds"
	};


	int count = static_cast<int>(fileName.size());

	for (int i = 0; i < count; i++)
	{
		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->Name = texName[i];
		texture->Filename = fileName[i];
		CHECK(::CreateDDSTextureFromFile12(_device.Get(), _cmdList.Get(), texture->Filename.c_str(), texture->Resource, texture->UploadHeap));
		_textures[texture->Name] = ::move(texture);
	}
}

void DynamicCubeDemo::BuildRootSignature()
{

	CD3DX12_DESCRIPTOR_RANGE texcubeTable;
	texcubeTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0);

	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 5, 1, 0);

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER params[5];

	params[0].InitAsConstantBufferView(0);
	params[1].InitAsConstantBufferView(1);
	params[2].InitAsShaderResourceView(0, 1);
	params[3].InitAsDescriptorTable(1, &texcubeTable, D3D12_SHADER_VISIBILITY_PIXEL);
	params[4].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	

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

void DynamicCubeDemo::BuildDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 6;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap)));

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(_srvHeap->GetCPUDescriptorHandleForHeapStart());

	auto bricksTex = _textures["bricksTex"]->Resource;
	auto tileTex = _textures["tileTex"]->Resource;
	auto defaultTex = _textures["defaultTex"]->Resource;
	auto skycube = _textures["skyCube"]->Resource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = bricksTex->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = bricksTex->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	_device->CreateShaderResourceView(bricksTex.Get(), &srvDesc, hDescriptor);


	hDescriptor.Offset(1, _cbvSrvDescriptorSize);

	srvDesc.Format = tileTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = tileTex->GetDesc().MipLevels;
	_device->CreateShaderResourceView(tileTex.Get(), &srvDesc, hDescriptor);


	hDescriptor.Offset(1, _cbvSrvDescriptorSize);

	srvDesc.Format = defaultTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = defaultTex->GetDesc().MipLevels;
	_device->CreateShaderResourceView(defaultTex.Get(), &srvDesc, hDescriptor);


	hDescriptor.Offset(1, _cbvSrvDescriptorSize);


	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = skycube->GetDesc().MipLevels;
	srvDesc.TextureCube.ResourceMinLODClamp = 0.f;
	srvDesc.Format = skycube->GetDesc().Format;
	_device->CreateShaderResourceView(skycube.Get(), &srvDesc, hDescriptor);

	auto srvCpuStart = _srvHeap->GetCPUDescriptorHandleForHeapStart();
	auto srvGpuStart = _srvHeap->GetGPUDescriptorHandleForHeapStart();
	auto rtvCputStart = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	int swapChainOffset = _swapChainCount;


	_skyTexHeapIndex = 3;
	_dynamicTexHeapIndex = _skyTexHeapIndex + 1;

	CD3DX12_CPU_DESCRIPTOR_HANDLE cubertvHandles[6];
	for (int i = 0; i < 6; ++i)
		cubertvHandles[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvCputStart, swapChainOffset + i, _rtvDescriptorSize);

	_dynamicCubeMap->BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE(srvCpuStart, _dynamicTexHeapIndex, _cbvSrvUavDescriptorSize),
		CD3DX12_GPU_DESCRIPTOR_HANDLE(srvGpuStart, _dynamicTexHeapIndex, _cbvSrvUavDescriptorSize),
		cubertvHandles
	);
}

void DynamicCubeDemo::BuildCubeDepthStencil()
{
	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = CubeMapSize;
	depthStencilDesc.Height = CubeMapSize;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = _depthStencilFormat;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = _depthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;


	auto property = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ThrowIfFailed(_device->CreateCommittedResource(
		&property,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(_cubeDepthStencilBuffer.GetAddressOf())));

	_device->CreateDepthStencilView(_cubeDepthStencilBuffer.Get(), nullptr, _cubedsvhandle);

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(_cubeDepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);

	_cmdList->ResourceBarrier(1, &barrier);
}

void DynamicCubeDemo::BuildShadersAndInputLayout()
{
	const D3D_SHADER_MACRO alphaTestDefines[] =
	{
		"ALPHA_TEST", "1",
		NULL, NULL
	};

	_shaders["defaultVS"] = d3dUtil::CompileShader(L"..\\Shaders\\21. Cubemap.fx", nullptr, "VS", "vs_5_1");
	_shaders["defaultPS"] = d3dUtil::CompileShader(L"..\\Shaders\\21. Cubemap.fx", nullptr, "PS", "ps_5_1");

	_shaders["skyVS"] = d3dUtil::CompileShader(L"..\\Shaders\\SkyCube.fx", nullptr, "VS", "vs_5_1");
	_shaders["skyPS"] = d3dUtil::CompileShader(L"..\\Shaders\\SkyCube.fx", nullptr, "PS", "ps_5_1");

	_inputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
}

void DynamicCubeDemo::BuildMeshGeometry()
{
	GeometryGenerator geoGen;
	MeshData box = geoGen.CreateBox(1.0f, 1.0f, 1.0f, 3);
	MeshData grid = geoGen.CreateGrid(20.0f, 30.0f, 60, 40);
	MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
	MeshData cylinder = geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20);

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	UINT boxVertexOffset = 0;
	UINT gridVertexOffset = (UINT)box.Vertices.size();
	UINT sphereVertexOffset = gridVertexOffset + (UINT)grid.Vertices.size();
	UINT cylinderVertexOffset = sphereVertexOffset + (UINT)sphere.Vertices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	UINT boxIndexOffset = 0;
	UINT gridIndexOffset = (UINT)box.Indices32.size();
	UINT sphereIndexOffset = gridIndexOffset + (UINT)grid.Indices32.size();
	UINT cylinderIndexOffset = sphereIndexOffset + (UINT)sphere.Indices32.size();

	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;

	SubmeshGeometry gridSubmesh;
	gridSubmesh.IndexCount = (UINT)grid.Indices32.size();
	gridSubmesh.StartIndexLocation = gridIndexOffset;
	gridSubmesh.BaseVertexLocation = gridVertexOffset;

	SubmeshGeometry sphereSubmesh;
	sphereSubmesh.IndexCount = (UINT)sphere.Indices32.size();
	sphereSubmesh.StartIndexLocation = sphereIndexOffset;
	sphereSubmesh.BaseVertexLocation = sphereVertexOffset;

	SubmeshGeometry cylinderSubmesh;
	cylinderSubmesh.IndexCount = (UINT)cylinder.Indices32.size();
	cylinderSubmesh.StartIndexLocation = cylinderIndexOffset;
	cylinderSubmesh.BaseVertexLocation = cylinderVertexOffset;

	auto totalVertexCount =
		box.Vertices.size() +
		grid.Vertices.size() +
		sphere.Vertices.size() +
		cylinder.Vertices.size();

	std::vector<VertexTextureNormalData> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].position = box.Vertices[i].position;
		vertices[k].normal = box.Vertices[i].normal;
		vertices[k].uv = box.Vertices[i].uv;
	}

	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].position = grid.Vertices[i].position;
		vertices[k].normal = grid.Vertices[i].normal;
		vertices[k].uv = grid.Vertices[i].uv;
	}

	for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].position = sphere.Vertices[i].position;
		vertices[k].normal = sphere.Vertices[i].normal;
		vertices[k].uv = sphere.Vertices[i].uv;
	}

	for (size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].position = cylinder.Vertices[i].position;
		vertices[k].normal = cylinder.Vertices[i].normal;
		vertices[k].uv = cylinder.Vertices[i].uv;
	}

	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(box.GetIndices16()), std::end(box.GetIndices16()));
	indices.insert(indices.end(), std::begin(grid.GetIndices16()), std::end(grid.GetIndices16()));
	indices.insert(indices.end(), std::begin(sphere.GetIndices16()), std::end(sphere.GetIndices16()));
	indices.insert(indices.end(), std::begin(cylinder.GetIndices16()), std::end(cylinder.GetIndices16()));

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(VertexTextureNormalData);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = make_shared<MeshGeometry>();
	geo->Name = "shapeGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(_device.Get(),
		_cmdList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(_device.Get(),
		_cmdList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(VertexTextureNormalData);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs["box"] = boxSubmesh;
	geo->DrawArgs["grid"] = gridSubmesh;
	geo->DrawArgs["sphere"] = sphereSubmesh;
	geo->DrawArgs["cylinder"] = cylinderSubmesh;

	_geometry[geo->Name] = std::move(geo);

}

void DynamicCubeDemo::BuildCarGeometry()
{
	std::ifstream fin("../Resources/Models/car.txt");

	if (!fin)
	{
		MessageBox(0, L"Models/car.txt not found.", 0, 0);
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

		vertices[i].uv = { 0.0f, 0.0f };

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
	geometry->Name = "carGeo";

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

	geometry->DrawArgs["car"] = submesh;
	_geometry[geometry->Name] = ::move(geometry);

}

void DynamicCubeDemo::BuildSkullGeometry()
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



void DynamicCubeDemo::BuildMaterials()
{
	auto bricks0 = make_shared<Material>();
	bricks0->Name = "bricks0";
	bricks0->MatCBIndex = 0;
	bricks0->DiffuseSrvHeapIndex = 0;
	bricks0->DiffuseAlbedo = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	bricks0->FresnelR0 = Vec3(0.02f, 0.02f, 0.02f);
	bricks0->Roughness = 0.1f;

	auto tile0 = make_shared<Material>();
	tile0->Name = "tile0";
	tile0->MatCBIndex = 1;
	tile0->DiffuseSrvHeapIndex = 1;
	tile0->DiffuseAlbedo = Vec4(0.9f, 0.9f, 0.9f, 1.0f);
	tile0->FresnelR0 = Vec3(0.2f, 0.2f, 0.2f);
	tile0->Roughness = 0.1f;

	auto mirror0 = make_shared<Material>();
	mirror0->Name = "mirror0";
	mirror0->MatCBIndex = 2;
	mirror0->DiffuseSrvHeapIndex = 2;
	mirror0->DiffuseAlbedo = Vec4(0.0f, 0.0f, 0.1f, 1.0f);
	mirror0->FresnelR0 = Vec3(0.98f, 0.97f, 0.95f);
	mirror0->Roughness = 0.1f;

	auto skullMat = make_shared<Material>();
	skullMat->Name = "skullMat";
	skullMat->MatCBIndex = 3;
	skullMat->DiffuseSrvHeapIndex = 2;
	skullMat->DiffuseAlbedo = Vec4(0.8f, 0.8f, 0.8f, 1.0f);
	skullMat->FresnelR0 = Vec3(0.2f, 0.2f, 0.2f);
	skullMat->Roughness = 0.2f;

	auto sky = std::make_shared<Material>();
	sky->Name = "sky";
	sky->MatCBIndex = 4;
	sky->DiffuseSrvHeapIndex = 3;
	sky->DiffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	sky->FresnelR0 = XMFLOAT3(0.1f, 0.1f, 0.1f);
	sky->Roughness = 1.0f;

	_material["bricks0"] = ::move(bricks0);
	_material["tile0"] = ::move(tile0);
	_material["mirror0"] = ::move(mirror0);
	_material["skullMat"] = ::move(skullMat);
	_material["sky"] = ::move(sky);
}

void DynamicCubeDemo::BuildRenderItems()
{
	auto shapeGeo = _geometry["shapeGeo"];
	auto skullGeo = _geometry["skullGeo"];

	{
		
		auto sphereMesh = shapeGeo->DrawArgs["sphere"];
		auto skyMaterial = _material["sky"];

		auto skyobject = make_shared<RenderObject>();
		skyobject->SetWorldMatrix(Matrix::CreateScale(5000.0f, 5000.0f, 5000.0f));
		skyobject->SetTexMatrix(MathHelper::Identity4x4());
		skyobject->SetConstantIndex(0);
		skyobject->SetMaterial(skyMaterial);
		skyobject->SetGeometry(shapeGeo);
		skyobject->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		skyobject->SetIndexCount(sphereMesh.IndexCount);
		skyobject->SetStartIndexLocation(sphereMesh.StartIndexLocation);
		skyobject->SetBaseIndexLocation(sphereMesh.BaseVertexLocation);

		_renderItems[static_cast<int>(RenderLayer::Sky)].push_back(skyobject);
		_gameObjects.push_back(::move(skyobject));
	}
	
	{
		auto boxMesh = shapeGeo->DrawArgs["box"];
		auto bricksMat = _material["bricks0"];

		auto boxObject = make_shared<RenderObject>();
		boxObject->SetWorldMatrix(Matrix::CreateScale(2.f, 1.f, 2.f) * Matrix::CreateTranslation(0.f, 0.5f, 0.f));
		boxObject->SetTexMatrix(Matrix::CreateScale(1.f, 1.f, 1.f));
		boxObject->SetConstantIndex(1);
		boxObject->SetMaterial(bricksMat);
		boxObject->SetGeometry(shapeGeo);
		boxObject->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		boxObject->SetIndexCount(boxMesh.IndexCount);
		boxObject->SetStartIndexLocation(boxMesh.StartIndexLocation);
		boxObject->SetBaseIndexLocation(boxMesh.BaseVertexLocation);

		_renderItems[static_cast<int>(RenderLayer::Opaque)].push_back(boxObject);
		_gameObjects.push_back(::move(boxObject));
	}

	{
		auto skullmesh = skullGeo->DrawArgs["skull"];
		auto skullmat = _material["skullMat"];

		auto skullObject = make_shared<RenderObject>();
		skullObject->SetWorldMatrix(Matrix::CreateScale(0.4f, 0.4f, 0.4f) * Matrix::CreateTranslation(0.f, 1.f, 0.f));
		skullObject->SetTexMatrix(MathHelper::Identity4x4());
		skullObject->SetConstantIndex(2);
		skullObject->SetMaterial(skullmat);
		skullObject->SetGeometry(skullGeo);
		skullObject->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		skullObject->SetIndexCount(skullmesh.IndexCount);
		skullObject->SetStartIndexLocation(skullmesh.StartIndexLocation);
		skullObject->SetBaseIndexLocation(skullmesh.BaseVertexLocation);

		_renderItems[static_cast<int>(RenderLayer::Opaque)].push_back(skullObject);
		_skullItem = skullObject;
		_gameObjects.push_back(skullObject);
	}


	{
		auto shpereMesh = shapeGeo->DrawArgs["sphere"];
		auto globeMat = _material["mirror0"];

		auto reflectObject = make_shared<RenderObject>();
		reflectObject->SetWorldMatrix(Matrix::CreateScale(2.f, 2.f, 2.f) * Matrix::CreateTranslation(0.f, 2.f, 0.f));
		reflectObject->SetTexMatrix(Matrix::CreateScale(1.f, 1.f, 1.f));
		reflectObject->SetConstantIndex(3);
		reflectObject->SetMaterial(globeMat);
		reflectObject->SetGeometry(shapeGeo);
		reflectObject->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		reflectObject->SetIndexCount(shpereMesh.IndexCount);
		reflectObject->SetStartIndexLocation(shpereMesh.StartIndexLocation);
		reflectObject->SetBaseIndexLocation(shpereMesh.BaseVertexLocation);

		_renderItems[static_cast<int>(RenderLayer::OpaqueDynamicReflectors)].push_back(reflectObject);
		_gameObjects.push_back(::move(reflectObject));
	}

	{
		auto gridMesh = shapeGeo->DrawArgs["grid"];
		auto gridMat = _material["tile0"];

		auto gridObject = make_shared<RenderObject>();
		gridObject->SetWorldMatrix(MathHelper::Identity4x4());
		gridObject->SetTexMatrix(Matrix::CreateScale(8.f, 8.f, 1.f));
		gridObject->SetConstantIndex(4);
		gridObject->SetMaterial(gridMat);
		gridObject->SetGeometry(shapeGeo);
		gridObject->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gridObject->SetIndexCount(gridMesh.IndexCount);
		gridObject->SetStartIndexLocation(gridMesh.StartIndexLocation);
		gridObject->SetBaseIndexLocation(gridMesh.BaseVertexLocation);

		_renderItems[static_cast<int>(RenderLayer::Opaque)].push_back(gridObject);
		_gameObjects.push_back(::move(gridObject));
	}


	Matrix brickTexTransform = XMMatrixScaling(1.5f, 2.0f, 1.0f);
	UINT objCBIndex = 5;

	auto mirrorMat = _material["mirror0"];
	auto brickMat = _material["bricks0"];

	auto cylinder = shapeGeo->DrawArgs["cylinder"];
	auto sphere = shapeGeo->DrawArgs["sphere"];

	for (int i = 0; i < 5; ++i)
	{
		auto leftCylRitem = make_shared<RenderObject>();;
		auto rightCylRitem = make_shared<RenderObject>();
		auto leftSphereRitem = make_shared<RenderObject>();
		auto rightSphereRitem = make_shared<RenderObject>();

		Matrix leftCylWorld = XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f);
		Matrix rightCylWorld = XMMatrixTranslation(+5.0f, 1.5f, -10.0f + i * 5.0f);

		Matrix leftSphereWorld = XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f);
		Matrix rightSphereWorld = XMMatrixTranslation(+5.0f, 3.5f, -10.0f + i * 5.0f);

		


		leftCylRitem->SetWorldMatrix(rightCylWorld);
		leftCylRitem->SetTexMatrix(brickTexTransform);
		leftCylRitem->SetConstantIndex(objCBIndex++);
		leftCylRitem->SetMaterial(brickMat);
		leftCylRitem->SetGeometry(shapeGeo);
		leftCylRitem->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		leftCylRitem->SetIndexCount(cylinder.IndexCount);
		leftCylRitem->SetStartIndexLocation(cylinder.StartIndexLocation);
		leftCylRitem->SetBaseIndexLocation(cylinder.BaseVertexLocation);


		rightCylRitem->SetWorldMatrix(leftCylWorld);
		rightCylRitem->SetTexMatrix(brickTexTransform);
		rightCylRitem->SetConstantIndex(objCBIndex++);
		rightCylRitem->SetMaterial(brickMat);
		rightCylRitem->SetGeometry(shapeGeo);
		rightCylRitem->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		rightCylRitem->SetIndexCount(cylinder.IndexCount);
		rightCylRitem->SetStartIndexLocation(cylinder.StartIndexLocation);
		rightCylRitem->SetBaseIndexLocation(cylinder.BaseVertexLocation);



		leftSphereRitem->SetWorldMatrix(leftSphereWorld);
		leftSphereRitem->SetTexMatrix(MathHelper::Identity4x4());
		leftSphereRitem->SetConstantIndex(objCBIndex++);
		leftSphereRitem->SetMaterial(mirrorMat);
		leftSphereRitem->SetGeometry(shapeGeo);
		leftSphereRitem->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		leftSphereRitem->SetIndexCount(sphere.IndexCount);
		leftSphereRitem->SetStartIndexLocation(sphere.StartIndexLocation);
		leftSphereRitem->SetBaseIndexLocation(sphere.BaseVertexLocation);



		rightSphereRitem->SetWorldMatrix(rightSphereWorld);
		rightSphereRitem->SetTexMatrix(MathHelper::Identity4x4());
		rightSphereRitem->SetConstantIndex(objCBIndex++);
		rightSphereRitem->SetMaterial(mirrorMat);
		rightSphereRitem->SetGeometry(shapeGeo);
		rightSphereRitem->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		rightSphereRitem->SetIndexCount(sphere.IndexCount);
		rightSphereRitem->SetStartIndexLocation(sphere.StartIndexLocation);
		rightSphereRitem->SetBaseIndexLocation(sphere.BaseVertexLocation);



		_renderItems[(int)RenderLayer::Opaque].push_back(leftCylRitem);
		_renderItems[(int)RenderLayer::Opaque].push_back(rightCylRitem);
		_renderItems[(int)RenderLayer::Opaque].push_back(leftSphereRitem);
		_renderItems[(int)RenderLayer::Opaque].push_back(rightSphereRitem);

		_gameObjects.push_back(leftCylRitem);
		_gameObjects.push_back(rightCylRitem);
		_gameObjects.push_back(leftSphereRitem);
		_gameObjects.push_back(rightSphereRitem);
	}

	
}

void DynamicCubeDemo::BuildFrameResources()
{
	for (int i = 0; i < gNumFrameResources; i++)
	{
		shared_ptr<FrameResource> resources = make_shared<FrameResource>(_device.Get(), 7, static_cast<UINT>(_gameObjects.size()), static_cast<UINT>(_material.size()));
		_frameResources.push_back(resources);
	}
}

void DynamicCubeDemo::BuildPSOs()
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


	D3D12_GRAPHICS_PIPELINE_STATE_DESC skypipeline = pipelineStates;
	skypipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	skypipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	skypipeline.pRootSignature = _rootSignature.Get();

	skypipeline.VS = { reinterpret_cast<BYTE*>(_shaders["skyVS"]->GetBufferPointer()), _shaders["skyVS"]->GetBufferSize() };
	skypipeline.PS = { reinterpret_cast<BYTE*>(_shaders["skyPS"]->GetBufferPointer()), _shaders["skyPS"]->GetBufferSize() };

	ThrowIfFailed(_device->CreateGraphicsPipelineState(&skypipeline, IID_PPV_ARGS(&_pipelineStates["sky"])));
}

void DynamicCubeDemo::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<shared_ptr<RenderObject>>& ritems)
{
	UINT byteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	auto objectConstant = _currFrameResource->ObjectCB->Resource();

	// For each render item...
	for (size_t i = 0; i < ritems.size(); ++i)
	{
		auto gameObject = ritems[i];

		if (gameObject->GetVisible() == false)
			continue;

		auto vertexBuffer = gameObject->GetGeometry()->VertexBufferView();
		auto indexBuffer = gameObject->GetGeometry()->IndexBufferView();

		_cmdList->IASetVertexBuffers(0, 1, &vertexBuffer);
		_cmdList->IASetIndexBuffer(&indexBuffer);
		_cmdList->IASetPrimitiveTopology(gameObject->GetPrimitiveType());

		auto constantSize = gameObject->GetConstantIndex() * byteSize;

		D3D12_GPU_VIRTUAL_ADDRESS objaddress = objectConstant->GetGPUVirtualAddress() + constantSize;

		_cmdList->SetGraphicsRootConstantBufferView(1, objaddress);
		_cmdList->DrawIndexedInstanced(gameObject->GetIndexCount(), 1, gameObject->GetStartIndexLocation(), gameObject->GetBaseVertexLocation(), 0);
	}
}

void DynamicCubeDemo::DrawSceneToCubeMap()
{
	auto viewport = _dynamicCubeMap->Viewport();
	auto scissorRect = _dynamicCubeMap->ScissorRect();

	_cmdList->RSSetViewports(1, &viewport);
	_cmdList->RSSetScissorRects(1, &scissorRect);

	// Change to RENDER_TARGET.
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(_dynamicCubeMap->GetComPtr().Get(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
	_cmdList->ResourceBarrier(1, &barrier);

	UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));

	// For each cube map face.
	for (int i = 0; i < 6; ++i)
	{

		auto rtv = _dynamicCubeMap->GetRtv(i);
		// Clear the back buffer and depth buffer.
		_cmdList->ClearRenderTargetView(rtv, Colors::LightSteelBlue, 0, nullptr);
		_cmdList->ClearDepthStencilView(_cubedsvhandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		// Specify the buffers we are going to render to.
		_cmdList->OMSetRenderTargets(1, &rtv, true, &_cubedsvhandle);

		// Bind the pass constant buffer for this cube map face so we use 
		// the right view/proj matrix for this cube face.
		auto passCB = _currFrameResource->PassCB->Resource();
		D3D12_GPU_VIRTUAL_ADDRESS passCBAddress = passCB->GetGPUVirtualAddress() + (1 + i) * passCBByteSize;
		_cmdList->SetGraphicsRootConstantBufferView(0, passCBAddress);

		DrawRenderItems(_cmdList.Get(), _renderItems[(int)RenderLayer::Opaque]);

		_cmdList->SetPipelineState(_pipelineStates["sky"].Get());
		DrawRenderItems(_cmdList.Get(), _renderItems[(int)RenderLayer::Sky]);

		_cmdList->SetPipelineState(_pipelineStates["opaque"].Get());
	}

	// Change back to GENERIC_READ so we can read the texture in a shader.
	barrier = CD3DX12_RESOURCE_BARRIER::Transition(_dynamicCubeMap->GetComPtr().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);
	_cmdList->ResourceBarrier(1, &barrier);
}

void DynamicCubeDemo::BuildCubeFaceCamera(float x, float y, float z)
{
	// Generate the cube map about the given position.
	XMFLOAT3 center(x, y, z);
	XMFLOAT3 worldUp(0.0f, 1.0f, 0.0f);

	// Look along each coordinate axis.
	XMFLOAT3 targets[6] =
	{
		XMFLOAT3(x + 1.0f, y, z), // +X
		XMFLOAT3(x - 1.0f, y, z), // -X
		XMFLOAT3(x, y + 1.0f, z), // +Y
		XMFLOAT3(x, y - 1.0f, z), // -Y
		XMFLOAT3(x, y, z + 1.0f), // +Z
		XMFLOAT3(x, y, z - 1.0f)  // -Z
	};

	// Use world up vector (0,1,0) for all directions except +Y/-Y.  In these cases, we
	// are looking down +Y or -Y, so we need a different "up" vector.
	XMFLOAT3 ups[6] =
	{
		XMFLOAT3(0.0f, 1.0f, 0.0f),  // +X
		XMFLOAT3(0.0f, 1.0f, 0.0f),  // -X
		XMFLOAT3(0.0f, 0.0f, -1.0f), // +Y
		XMFLOAT3(0.0f, 0.0f, +1.0f), // -Y
		XMFLOAT3(0.0f, 1.0f, 0.0f),	 // +Z
		XMFLOAT3(0.0f, 1.0f, 0.0f)	 // -Z
	};

	for (int i = 0; i < 6; ++i)
	{
		_cubeMapCamera[i].LookAt(center, targets[i], ups[i]);
		_cubeMapCamera[i].SetLens(0.5f * XM_PI, 1.0f, 0.1f, 1000.0f);
		_cubeMapCamera[i].UpdateViewMatrix();
	}
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> DynamicCubeDemo::GetStaticSamplers()
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
