#pragma once
class TextureMultiple : public ResourceBase
{
	using Super = ResourceBase;

public:
	TextureMultiple();
	~TextureMultiple();

	void Init();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return _srvHandle; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return _gpuHandle; }



private:
	vector<shared_ptr<Texture>>		_textures;
	ComPtr<ID3D12DescriptorHeap>	_srvHeap;


	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE		_gpuHandle = {};

private:
	Vec2 _size = { 0.f, 0.f };
	Vec2 _texOffset = { 0, 0 };
};

