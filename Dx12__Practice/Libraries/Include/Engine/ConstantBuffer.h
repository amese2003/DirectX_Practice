#pragma once

enum class CONSTANT_BUFFER_TYPE : uint8
{
	GLOBAL,
	TRANSFORM,
	MATERIAL,
	END
};

enum
{
	CONSTANT_BUFFER_COUNT = static_cast<uint8>(CONSTANT_BUFFER_TYPE::END)
};

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	D3D12_CPU_DESCRIPTOR_HANDLE PushData(void* buffer, uint32 size);
	void SetGlobalData(void* buffer, uint32 size);
	void SetMaterialData(void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	void CreateBuffer();
	void CreateView();

private:
	ComPtr<ID3D12Resource> _constantBuffer;
	BYTE* _mappedBuffer = nullptr;
	uint32 _elementSize = 0;
	uint32 _elementCount = 0;

	ComPtr<ID3D12DescriptorHeap> _cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE	_cpuHandleBegin = {};
	uint32 _handleIncrementSize = 0;

	uint32 _currentIndex = 0;

	CBV_REGISTER _reg = {};
};

