#include "pch.h"
#include "ComputeTexture.h"

ComputeTexture::ComputeTexture() : Super(ResourceType::ComputeTexture)
{
}

ComputeTexture::~ComputeTexture()
{
}

void ComputeTexture::CreateUAVTexture(UINT64 byteSize)
{
}

void ComputeTexture::CreateCopyTexture(UINT64 byteSize)
{
}

void ComputeTexture::CreateTexture()
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = textureDescriptorCount + blurDescriptorCount;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap));

	_currentCount = 1;
	_descriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

void ComputeTexture::PushData(ComPtr<ID3D12Resource> texture)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(_srvHeap->GetCPUDescriptorHandleForHeapStart());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = texture->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	DEVICE->CreateShaderResourceView(texture.Get(), &srvDesc, hDescriptor);

	_currentCount++;

	if (_currentCount < blurDescriptorCount)
		hDescriptor.Offset(1, _descriptorSize);
}

void ComputeTexture::CommitTexture()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuhandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_srvHeap->GetCPUDescriptorHandleForHeapStart(), 3, _descriptorSize);
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuhandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(_srvHeap->GetGPUDescriptorHandleForHeapStart(), 3, _descriptorSize);

}

void ComputeTexture::CreateFromTexture(ComPtr<ID3D12Resource> tex2D)
{
}

