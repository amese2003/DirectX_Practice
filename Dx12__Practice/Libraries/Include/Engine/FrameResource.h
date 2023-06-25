#pragma once

class ContantBuffer;


class FrameResource : public ResourceBase
{
	using Super = ResourceBase;

public:
    FrameResource();
    ~FrameResource();

    void Init(ID3D12Device* device, UINT passCount, UINT maxInstanceCount, UINT materialCount);

private:
    ComPtr<ID3D12CommandAllocator> _cmdAlloc;

    unique_ptr<UploadBuffer<PassConstants>> _globalBuffer = nullptr;
    unique_ptr<UploadBuffer<MaterialData>> _materialBuffer = nullptr;
    unique_ptr<UploadBuffer<InstanceData>> _instanceBuffer = nullptr;

    UINT64 Fence = 0;
private:


};

