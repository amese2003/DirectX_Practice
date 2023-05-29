#pragma once
class RootSignature
{
public:
	void Init();
	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }
	ComPtr<ID3D12RootSignature> GetComputeSignature() { return _computeSignature; }
private:
	void CreateSamplerDesc();
	void CreateRootSignature();
	void CreateComputeSignature();

private:
	ComPtr<ID3D12RootSignature>	_signature;
	ComPtr<ID3D12RootSignature>	_computeSignature;

	D3D12_STATIC_SAMPLER_DESC _samplerDesc;
};
