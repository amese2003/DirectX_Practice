#pragma once
class ComputeTexture : public ResourceBase
{
	using Super = ResourceBase;

public:
	ComputeTexture();
	~ComputeTexture();

	//void Init(const wstring& path, bool isArray = false);
	ComPtr<ID3D12Resource> GetComPtr() { return _texture2D; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return _srvHandle; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return _gpuHandle; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetUavCpuHandle() { return _uavHandle; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetUavGpuHandle() { return _uavgpuHandle; }


	void CreateUAVTexture(UINT64 byteSize);
	void CreateCopyTexture(UINT64 byteSize);

public:
	void CreateTexture();
	void PushData(ComPtr<ID3D12Resource> texture);
	void CommitTexture();

	void CreateFromTexture(ComPtr<ID3D12Resource> tex2D);


	float GetWidth() { return static_cast<float>(_desc.Width); }
	float GetHeight() { return static_cast<float>(_desc.Height); }

	Vec2 GetOffset() { return _texOffset; }
	void SetOffset(Vec2 offset) { _texOffset = offset; }



private:
	ScratchImage				_image;
	D3D12_RESOURCE_DESC			_desc;
	ComPtr<ID3D12Resource>		_texture2D;
	ComPtr<ID3D12Resource>		_uploadBuffer;


	ComPtr<ID3D12DescriptorHeap>	_srvHeap;
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	ComPtr<ID3D12DescriptorHeap>	_dsvHeap;
	ComPtr<ID3D12DescriptorHeap>	_uavHeap;


	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE		_gpuHandle = {};

	D3D12_CPU_DESCRIPTOR_HANDLE		_uavHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE		_uavgpuHandle = {};

private:
	Vec2 _size = { 0.f, 0.f };
	Vec2 _texOffset = { 0, 0 };

	UINT _descriptorSize = 0;

	int _currentCount = 0;
	const int textureDescriptorCount = 3;
	const int blurDescriptorCount = 4;
};

