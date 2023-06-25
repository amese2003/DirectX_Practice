#include "pch.h"
#include "Texture.h"

Texture::Texture() : Super(ResourceType::Texture)
{
}

Texture::~Texture()
{
}


void Texture::Load(const wstring& path)
{
	DirectX::TexMetadata md;
	HRESULT hr;

	wstring ext = fs::path(path).extension();
	/*hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, _image);*/

	if (ext == L".dds" || ext == L".DDS")
		hr = ::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, &md, _image);
	else if (ext == L".tga" || ext == L".TGA")
		hr = ::LoadFromTGAFile(path.c_str(), &md, _image);
	else // png, jpg, jpeg, bmp
		hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, _image);

	CHECK(hr);



	hr = ::CreateTexture(DEVICE.Get(), _image.GetMetadata(), &_texture2D);
	CHECK(hr);

	vector<D3D12_SUBRESOURCE_DATA> subResources;

	hr = ::PrepareUpload(DEVICE.Get(),
		_image.GetImages(),
		_image.GetImageCount(),
		_image.GetMetadata(),
		subResources);

	CHECK(hr);

	const uint64 bufferSize = ::GetRequiredIntermediateSize(_texture2D.Get(), 0, static_cast<uint32>(subResources.size()));

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ComPtr<ID3D12Resource> textureUploadHeap;
	hr = DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureUploadHeap.GetAddressOf()));

	CHECK(hr);

	::UpdateSubresources(RESOURCE_CMD_LIST.Get(),
		_texture2D.Get(),
		textureUploadHeap.Get(),
		0, 0,
		static_cast<unsigned int>(subResources.size()),
		subResources.data());

	_size.x = md.width;
	_size.y = md.height;

	GRAPHICS->GetCmdQueue()->FlushResourceCommandQueue();
	CreateFromTexture(_texture2D);
}

void Texture::CreateComputeTexture(const void* data, UINT64 byteSize)
{

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);

	HRESULT hr;

	hr = DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(_texture2D.GetAddressOf()));

	CHECK(hr);

	heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	hr = DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(_uploadBuffer.GetAddressOf()));
	CHECK(hr);


	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = data;
	subResourceData.RowPitch = byteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(_texture2D.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);

	/*COMPUTE_CMD_LIST->ResourceBarrier(1, &barrier);

	::UpdateSubresources<1>(COMPUTE_CMD_LIST.Get(),
		_texture2D.Get(),
		_uploadBuffer.Get(),
		0, 0,
		1,
		&subResourceData);

	barrier = CD3DX12_RESOURCE_BARRIER::Transition(_texture2D.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	COMPUTE_CMD_LIST->ResourceBarrier(1, &barrier);*/
}

void Texture::CreateUAVTexture(UINT64 byteSize)
{
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(byteSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	HRESULT hr;
	// Create the buffer that will be a UAV.
	hr = DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(&_texture2D));

	CHECK(hr);
}

void Texture::CreateCopyTexture(UINT64 byteSize)
{
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);

	HRESULT hr;
	hr = DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&_texture2D));
	CHECK(hr);
}


void Texture::CreateTexture(DXGI_FORMAT format, uint32 width, uint32 height, const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor)
{
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
	desc.Flags = resFlags;


	D3D12_CLEAR_VALUE optimizedClearValue = {};
	D3D12_CLEAR_VALUE* pOptimizedClearValue = nullptr;

	D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;

	if (resFlags & D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
	{
		resourceStates = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
		//optimizedClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
		optimizedClearValue.Format = GRAPHICS->GetDepthStencilFormat();
		optimizedClearValue.DepthStencil.Depth = 1.0f;
		optimizedClearValue.DepthStencil.Stencil = 0;
		

		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = width;
		desc.Height = height;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		desc.SampleDesc.Count = GRAPHICS->Get4xMsaaState() ? 4 : 1;
		desc.SampleDesc.Quality = GRAPHICS->Get4xMsaaState() ? (GRAPHICS->Get4xMsaaLevel() - 1) : 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;


		pOptimizedClearValue = &optimizedClearValue;
	}
	else if (resFlags & D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
	{
		resourceStates = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
		float arrFloat[4] = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };
		optimizedClearValue = CD3DX12_CLEAR_VALUE(format, arrFloat);
		pOptimizedClearValue = &optimizedClearValue;
	}


	

	// Create Texture2D
	HRESULT hr = DEVICE->CreateCommittedResource(
		&heapProperty,
		heapFlags,
		&desc,
		resourceStates,
		pOptimizedClearValue,
		IID_PPV_ARGS(&_texture2D));

	assert(SUCCEEDED(hr));
	CreateFromTexture(_texture2D);
}

void Texture::CreateFromTexture(ComPtr<ID3D12Resource> tex2D)
{
	_texture2D = tex2D;

	D3D12_RESOURCE_DESC desc = tex2D->GetDesc();

	// 주요 조합
	// - DSV 단독 (조합X)
	// - SRV
	// - RTV + SRV
	if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
	{
		// DSV
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_dsvHeap));

		D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = _dsvHeap->GetCPUDescriptorHandleForHeapStart();

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = GRAPHICS->GetDepthStencilFormat();
		dsvDesc.Texture2D.MipSlice = 0;

		DEVICE->CreateDepthStencilView(_texture2D.Get(), &dsvDesc, hDSVHandle);
	}
	else
	{
		if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
		{
			// RTV
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			heapDesc.NumDescriptors = 1;
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			heapDesc.NodeMask = 0;
			DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_rtvHeap));

			D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
			DEVICE->CreateRenderTargetView(_texture2D.Get(), nullptr, rtvHeapBegin);
		}

		if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)
		{
			// UAV
			D3D12_DESCRIPTOR_HEAP_DESC uavHeapDesc = {};
			uavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			uavHeapDesc.NumDescriptors = 1;
			uavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			uavHeapDesc.NodeMask = 0;
			HRESULT hr;
			hr = DEVICE->CreateDescriptorHeap(&uavHeapDesc, IID_PPV_ARGS(&_uavHeap));
			CHECK(hr);

			_uavHandle = _uavHeap->GetCPUDescriptorHandleForHeapStart();
			_uavgpuHandle = _uavHeap->GetGPUDescriptorHandleForHeapStart();

			D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.Format = _image.GetMetadata().format;
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;

			DEVICE->CreateUnorderedAccessView(_texture2D.Get(), nullptr, &uavDesc, _uavHandle);
		}

		// SRV
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap));

		_srvHandle = _srvHeap->GetCPUDescriptorHandleForHeapStart();
		_gpuHandle = _srvHeap->GetGPUDescriptorHandleForHeapStart();

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = _image.GetMetadata().format;

		if (_image.GetMetadata().arraySize > 1)
		{
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.MostDetailedMip = 0;
			srvDesc.Texture2DArray.MipLevels = -1;
			srvDesc.Texture2DArray.FirstArraySlice = 0;
			srvDesc.Texture2DArray.ArraySize = _image.GetMetadata().arraySize;
		}
		else
		{
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
		}

		DEVICE->CreateShaderResourceView(_texture2D.Get(), &srvDesc, _srvHandle);
	}
}

