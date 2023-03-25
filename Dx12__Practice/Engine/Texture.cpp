#include "pch.h"
#include "Texture.h"

Texture::Texture() : Super(ResourceType::Texture)
{
}

Texture::~Texture()
{
}

void Texture::Init(const wstring& path)
{
	_srvHeap = GRAPHICS->GetTableDescHeap()->GetDescriptorHeap();

	CreateTexture(path);
	CreateView();
}

void Texture::CreateTexture(const wstring& path)
{
	DirectX::TexMetadata md;
	HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, _image);
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

	::UpdateSubresources(CMD_LIST.Get(),
		_texture2D.Get(),
		textureUploadHeap.Get(),
		0, 0,
		static_cast<unsigned int>(subResources.size()),
		subResources.data());

	_size.x = md.width;
	_size.y = md.height;

	GRAPHICS->GetCommandQueue()->FlushResourceCommandQueue();
}

void Texture::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap));

	_srvHandle = _srvHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = _image.GetMetadata().format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;
	DEVICE->CreateShaderResourceView(_texture2D.Get(), &srvDesc, _srvHandle);
}