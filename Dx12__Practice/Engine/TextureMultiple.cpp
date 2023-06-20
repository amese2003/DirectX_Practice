#include "pch.h"
#include "TextureMultiple.h"

TextureMultiple::TextureMultiple() : Super(ResourceType::TextureMultiple)
{
}

TextureMultiple::~TextureMultiple()
{
}

void TextureMultiple::Init(int Count, vector<shared_ptr<Texture>> textures)
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = textures.size();
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap));
	CHECK(hr);

	_textures = textures;
	auto descSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);;



	_srvHandle = _srvHeap->GetCPUDescriptorHandleForHeapStart();
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(_srvHandle);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textures[0]->GetComPtr()->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = textures[0]->GetComPtr()->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	DEVICE->CreateShaderResourceView(textures[0]->GetComPtr().Get(), &srvDesc, hDescriptor);


	for (int i = 1; i < textures.size(); i++)
	{
		hDescriptor.Offset(1, descSize);

		srvDesc.Format = textures[i]->GetComPtr()->GetDesc().Format;
		srvDesc.Texture2D.MipLevels = textures[i]->GetComPtr()->GetDesc().MipLevels;
		DEVICE->CreateShaderResourceView(textures[i]->GetComPtr().Get(), &srvDesc, hDescriptor);
	}
}
