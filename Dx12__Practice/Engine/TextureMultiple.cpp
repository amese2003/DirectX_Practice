#include "pch.h"
#include "TextureMultiple.h"

TextureMultiple::TextureMultiple() : Super(ResourceType::TextureMultiple)
{
}

TextureMultiple::~TextureMultiple()
{
}

void TextureMultiple::Init()
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 7;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	CHECK(DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap)));

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(_srvHeap->GetCPUDescriptorHandleForHeapStart());

	auto bricksTex = RESOURCES->Get<Texture>(L"brickTex");
	auto stoneTex = RESOURCES->Get<Texture>(L"stoneTex");
	auto tileTex = RESOURCES->Get<Texture>(L"tileTex");
	auto crateTex = RESOURCES->Get<Texture>(L"woodcreateTex");
	auto iceTex = RESOURCES->Get<Texture>(L"waterTex");
	auto grassTex = RESOURCES->Get<Texture>(L"grassTex");
	auto defaultTex = RESOURCES->Get<Texture>(L"whiteTex");

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = bricksTex->GetComPtr()->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = bricksTex->GetComPtr()->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	DEVICE->CreateShaderResourceView(bricksTex->GetComPtr().Get(), &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, GRAPHICS->GetDescSize());

	srvDesc.Format = stoneTex->GetComPtr()->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = stoneTex->GetComPtr()->GetDesc().MipLevels;
	DEVICE->CreateShaderResourceView(stoneTex->GetComPtr().Get(), &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, GRAPHICS->GetDescSize());

	srvDesc.Format = tileTex->GetComPtr()->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = tileTex->GetComPtr()->GetDesc().MipLevels;
	DEVICE->CreateShaderResourceView(tileTex->GetComPtr().Get(), &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, GRAPHICS->GetDescSize());

	srvDesc.Format = crateTex->GetComPtr()->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = crateTex->GetComPtr()->GetDesc().MipLevels;
	DEVICE->CreateShaderResourceView(crateTex->GetComPtr().Get(), &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, GRAPHICS->GetDescSize());

	srvDesc.Format = iceTex->GetComPtr()->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = iceTex->GetComPtr()->GetDesc().MipLevels;
	DEVICE->CreateShaderResourceView(iceTex->GetComPtr().Get(), &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, GRAPHICS->GetDescSize());

	srvDesc.Format = grassTex->GetComPtr()->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = grassTex->GetComPtr()->GetDesc().MipLevels;
	DEVICE->CreateShaderResourceView(grassTex->GetComPtr().Get(), &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, GRAPHICS->GetDescSize());

	srvDesc.Format = defaultTex->GetComPtr()->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = defaultTex->GetComPtr()->GetDesc().MipLevels;
	DEVICE->CreateShaderResourceView(defaultTex->GetComPtr().Get(), &srvDesc, hDescriptor);
}
