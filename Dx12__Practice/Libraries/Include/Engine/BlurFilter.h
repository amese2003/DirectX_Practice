#pragma once

class Decive;

class BlurFilter
{
public:
	BlurFilter();
	BlurFilter(const BlurFilter& rhs) = delete;
	BlurFilter& operator=(const BlurFilter& rhs) = delete;
	~BlurFilter();

	void Init(UINT width, UINT height, DXGI_FORMAT format);
	void BuildDescriptorHeap();
	void BuildExampleResource();
	void PushResource(ComPtr<ID3D12Resource> resource);
	void CommitResource();

	ComPtr<ID3D12Resource> GetOutput() { return _blurMap0; }
	void BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDescriptor,
		CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuDescriptor,
		UINT descriptorSize);

	
	void Update(UINT width, UINT height);
	void Execute(ComPtr<ID3D12GraphicsCommandList> cmdList, ComPtr<ID3D12Resource> input, int blurCount);
	shared_ptr<GameObject> _test;
private:
	shared_ptr<ComputeCommandQueue> _commandQueue;
	ComPtr<ID3D12RootSignature> _rootsignature;

private:
	vector<float> CalcGaussWeights(float sigma);

	void BuildDescriptors();
	void BuildResources();

private:
	const int MaxBlurRadius = 5;
	const int textureDescriptorCount = 3;
	const int blurDescriptorCount = 4;
	int _textureCount = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	UINT mWidth = 0;
	UINT mHeight = 0;
	DXGI_FORMAT mFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	CD3DX12_CPU_DESCRIPTOR_HANDLE mBlur0CpuSrv;
	CD3DX12_CPU_DESCRIPTOR_HANDLE mBlur0CpuUav;

	CD3DX12_CPU_DESCRIPTOR_HANDLE mBlur1CpuSrv;
	CD3DX12_CPU_DESCRIPTOR_HANDLE mBlur1CpuUav;

	CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur0GpuSrv;
	CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur0GpuUav;

	CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur1GpuSrv;
	CD3DX12_GPU_DESCRIPTOR_HANDLE mBlur1GpuUav;

	Microsoft::WRL::ComPtr<ID3D12Resource> _blurMap0 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> _blurMap1 = nullptr;

	ComPtr<ID3D12DescriptorHeap>	_srvHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE	_srvHandle;

	

};

