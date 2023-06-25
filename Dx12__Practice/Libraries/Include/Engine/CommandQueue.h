#pragma once

class SwapChain;

class CommandQueue
{
public:
	CommandQueue();
	~CommandQueue();

	void Init();
	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	void FlushResourceCommandQueue();
	void ExecuteCommandList();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return	_cmdList; }
	ComPtr<ID3D12CommandAllocator> GetAlloc() { return _cmdAlloc; }
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() { return	_resourceCmdList; }
	ComPtr<ID3D12CommandAllocator> GetResourceAlloc() { return	_resourceCmdAlloc; }


private:
	ComPtr<ID3D12CommandQueue>				_cmdQueue;
	ComPtr<ID3D12CommandAllocator>			_cmdAlloc;
	vector< ComPtr<ID3D12CommandAllocator>> _renderAlloc;
	ComPtr<ID3D12GraphicsCommandList>		_cmdList;

	// ¸®¼Ò½º
	ComPtr<ID3D12CommandAllocator>			_resourceCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>		_resourceCmdList;


	ComPtr<ID3D12Fence>						_fence;
	uint32									_fenceValue = 0;
	HANDLE									_fenceEvent = INVALID_HANDLE_VALUE;
};

