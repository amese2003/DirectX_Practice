#include "pch.h"
#include "FrameResource.h"

FrameResource::FrameResource() : Super(ResourceType::FrameResources)
{
}

FrameResource::~FrameResource()
{
}

void FrameResource::Init(ID3D12Device* device, UINT passCount, UINT maxInstanceCount, UINT materialCount)
{
	CHECK(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_cmdAlloc.GetAddressOf())));

	_globalBuffer = std::make_unique<UploadBuffer<PassConstants>>(device, passCount, true);
	_materialBuffer = std::make_unique<UploadBuffer<MaterialData>>(device, materialCount, false);
	_instanceBuffer = std::make_unique<UploadBuffer<InstanceData>>(device, maxInstanceCount, false);
}
