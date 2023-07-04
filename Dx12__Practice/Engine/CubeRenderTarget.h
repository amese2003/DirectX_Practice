#pragma once

enum class CubeMapFace : int
{
	PositiveX = 0,
	NegativeX = 1,
	PositiveY = 2,
	NegativeY = 3,
	PositiveZ = 4,
	NegativeZ = 5
};

const UINT CubeMapSize = 512;

class CubeRenderTarget
{
public:
	CubeRenderTarget(ComPtr<ID3D12Device> device, UINT width, UINT height, DXGI_FORMAT format);
	~CubeRenderTarget() {}

	ComPtr<ID3D12Resource> GetComPtr() { return _cubeMap; }

	CD3DX12_GPU_DESCRIPTOR_HANDLE GetSrv() { return _gpuHandle; }
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtv(int faceIndex) { return _rtvHandle[faceIndex]; }

	D3D12_VIEWPORT Viewport() const { return _viewport; };
	D3D12_RECT ScissorRect() const { return _scissorRect; };

	void BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuSrv,
		CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuSrv,
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuRtv[6]);

	void OnResize(UINT newWidth, UINT newHeight);

private:
	void BuildDescriptors();
	void BuildResource();

private:
	ComPtr<ID3D12Device> _device = nullptr;

	D3D12_VIEWPORT _viewport;
	D3D12_RECT _scissorRect;

	UINT _widht = 0;
	UINT _height = 0;
	DXGI_FORMAT _format = DXGI_FORMAT_R8G8B8A8_UNORM;

	CD3DX12_CPU_DESCRIPTOR_HANDLE _cpuHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE _gpuHandle;

	CD3DX12_CPU_DESCRIPTOR_HANDLE _rtvHandle[6];

	ComPtr<ID3D12Resource> _cubeMap = nullptr;

};

