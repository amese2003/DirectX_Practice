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


	Vec2 GetSize() { return _size; }

private:
	ScratchImage			 _image;
	ComPtr<ID3D12Resource>	_texture2D;


	ComPtr<ID3D12DescriptorHeap>	_srvHeap;
	// defered
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	ComPtr<ID3D12DescriptorHeap>	_dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHandle = {};


private:
	Vec2 _size = { 0.f, 0.f };
};

