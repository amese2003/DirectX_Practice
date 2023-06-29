#pragma once

class Device;
class CommandQueue;
class SwapChain;
class RootSignature;
class TableDescriptionHeap;
class ComputeDescriptorHeap;
class ConstantBuffer;
class DepthStencilBuffer;
class ComputeCommandQueue;
class BlurFilter;
class GameObject;
class FrameResource;

class Graphics
{
	DECLARE_SINGLE(Graphics);

public:
	void Init(HWND hwnd);

	void RenderBegin();;
	void RenderEnd();


	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<SwapChain> GetSwapChain() { return _swapChain; }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }
	shared_ptr<RootSignature> GetRootsignature() { return _rootSignature; }

	


public:
	HWND& GetWindow() { return _hwnd; }

	bool Get4xMsaaState() { return _4xMsaaState; }
	void Set4xMsaaState(bool state) { _4xMsaaState = state; }

	UINT Get4xMsaaLevel() { return _4xMsaaQuality; }
	void Set4xMsaaLevel(UINT level) { _4xMsaaQuality = level; }

	DXGI_FORMAT& GetBackBufferFormat() { return _backBufferFormat; }
	DXGI_FORMAT& GetDepthStencilFormat() { return _depthstencilFormat; }

	D3D12_VIEWPORT& GetViewport() { return _viewport; }
	void SetViewport();

	UINT GetDescSize() { return _cbvsrvdescSize; }
	int GetFrameResourceIdx() { return _frameResourceIndex; }

	shared_ptr<FrameResource> GetFrameResources() { return _frameResource; }

private:
	void LogAdapters();
	void LogAdapterOutputs(ComPtr<IDXGIAdapter> adapter);
	void LogOutputDisplayModes(ComPtr<IDXGIOutput> output, DXGI_FORMAT format);

private:
	shared_ptr<Device> _device;
	shared_ptr<SwapChain> _swapChain;
	shared_ptr<CommandQueue> _cmdQueue;
	shared_ptr<DepthStencilBuffer> _depthStencil;
	shared_ptr<RootSignature> _rootSignature;



private:
	HWND _hwnd = {};

	// Set true to use 4X MSAA (?.1.8).  The default is false.
	bool      _4xMsaaState = false;    // 4X MSAA enabled
	UINT      _4xMsaaQuality = 0;      // quality level of 4X MSAA

	std::wstring _mainCaption = L"d3d App";
	D3D_DRIVER_TYPE _driverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT _backBufferFormat =  DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT _depthstencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	UINT _cbvsrvdescSize = 0;

	D3D12_VIEWPORT _viewport = { 0 };
	D3D12_RECT		_scissorRect = {};

	shared_ptr<FrameResource> _frameResource;
	int _frameResourceIndex = 0;
};

