#pragma once


const int gNumFrameResources = 3;

class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	virtual ~GameApp();

	static GameApp* GetApp();

	HINSTANCE AppInst() const { return _appInst; }
	HWND GetMainWnd() const { return _mainWnd; }
	float AspectRatio() { return static_cast<float>(_clientWidth) / _clientHeight; }

	bool Get4xMsaaState() const { return _4xMsaaState; }
	void Set4xMsaaState(bool value);

	int Run();

	virtual bool Init();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	ComPtr<ID3D12Device> GetDevice() { return _device; }
protected:
	virtual void CreateRtvAndDsvDescriptorHeaps();
	virtual void OnResize();
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y) { }

protected:
	bool InitMainWindow();
	bool InitDirect3D();
	void CreateCommandObjects();
	void CreateSwapChain();

	void WaitAsync();

	ComPtr<ID3D12Resource> GetBackBuffer() const { return _swapChainBuffer[_curBackbuffer]; }
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const { return CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), _curBackbuffer, _rtvDescriptorSize); };
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const { return _dsvHeap->GetCPUDescriptorHandleForHeapStart(); };

	void CalculateFrameStats();

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:
	static GameApp* _app;

	HINSTANCE _appInst = nullptr;
	HWND _mainWnd = nullptr;
	bool _appPause = false;
	bool _minimize = false;
	bool _maximize = false;
	bool _resizing = false;
	bool _fullscreen = false;


	bool _4xMsaaState = false;
	UINT _4xMassQuality = 0;

protected:
	ComPtr<ID3D12Device>		_device;
	ComPtr<IDXGISwapChain>		_swapChain;
	ComPtr<IDXGIFactory4>		_dxgiFactory;

	ComPtr<ID3D12Fence>			_fence;
	UINT64						_currentFence = 0;
	HANDLE						_fenceEvent;

	ComPtr<ID3D12CommandQueue>			_cmdQueue;
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	static const int _swapChainCount = 2;
	int _curBackbuffer = 0;

	ComPtr<ID3D12Resource> _swapChainBuffer[_swapChainCount];
	ComPtr<ID3D12Resource> _depthStencilBuffer;

	ComPtr<ID3D12DescriptorHeap> _rtvHeap;
	ComPtr<ID3D12DescriptorHeap> _dsvHeap;

	D3D12_VIEWPORT _screenViewport;
	D3D12_RECT _scissorRect;

	UINT _rtvDescriptorSize = 0;
	UINT _dsvDescriptorSize = 0;
	UINT _cbvSrvUavDescriptorSize = 0;


	std::wstring _mainCaption = L"d3d App";
	D3D_DRIVER_TYPE _driverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT _backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT _depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int _clientWidth = 800;
	int _clientHeight = 600;

};

