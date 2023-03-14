#pragma once

class Device;
class CommandQueue;
class SwapChain;
class RootSignature;
class TableDescriptionHeap;
class ConstantBuffer;

class Graphics
{
	DECLARE_SINGLE(Graphics);

public:
	void Init(HWND hwnd);

	void RenderBegin();
	void RenderEnd();

public:
	shared_ptr<Device>					GetDevice() { return _device; }
	shared_ptr<CommandQueue>			GetCommandQueue() { return _cmdQueue; }
	shared_ptr<SwapChain>				GetSwapChain() { return _swapChain; }
	shared_ptr<RootSignature>			GetRootsignature() { return _rootSignature; }
	shared_ptr<TableDescriptionHeap>	GetTableDescHeap() { return _tableDescHeap; }
	shared_ptr<ConstantBuffer>			GetConstantBuffer() { return _constantBuffer; }

private:

	void SetViewport();

private:
	HWND _hwnd = {};


	shared_ptr<Device>					_device;
	shared_ptr<CommandQueue>			_cmdQueue;
	shared_ptr<SwapChain>				_swapChain;
	shared_ptr<RootSignature>			_rootSignature;
	shared_ptr<TableDescriptionHeap>	_tableDescHeap;
	shared_ptr<ConstantBuffer>			_constantBuffer;

	D3D12_VIEWPORT _viewport = { 0 };
	D3D12_RECT		_scissorRect = {};
};

