#pragma once
class Texture : public ResourceBase
{
	using Super = ResourceBase;

public:
	Texture();
	~Texture();

	void Init(const wstring& path);
	ComPtr<ID3D12Resource> GetComPtr() { return _texture2D; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return _srvHandle; }
	
	void CreateTexture(const wstring& path);
	void CreateView();


	float GetWidth() { return static_cast<float>(_desc.Width); }
	float GetHeight() { return static_cast<float>(_desc.Height); }

	Vec2 GetOffset() { return _texOffset; }
	void SetOffset(Vec2 offset) { _texOffset = offset; }

private:
	ScratchImage				_image;
	D3D12_RESOURCE_DESC			_desc;
	ComPtr<ID3D12Resource>		_texture2D;


	ComPtr<ID3D12DescriptorHeap>	_srvHeap;
	// defered
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	ComPtr<ID3D12DescriptorHeap>	_dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHandle = {};


private:
	Vec2 _size = { 0.f, 0.f };
	Vec2 _texOffset = { 0, 0 };
};

