#include "pch.h"
#include "Graphics.h"

void Graphics::Init(HWND hwnd)
{
	_hwnd = hwnd;

	SetViewport();

	_device = make_shared<Device>();
	_cmdQueue = make_shared<CommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_rootSignature = make_shared<RootSignature>();
	_tableDescHeap = make_shared<TableDescriptionHeap>();

	_constantBuffer = vector<shared_ptr<ConstantBuffer>>(static_cast<uint8>(CBV_REGISTER_COUNT));

	


	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(hwnd, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init(_device->GetDevice());
	_tableDescHeap->Init(256);

	_constantBuffer[static_cast<uint8>(CBV_REGISTER::b0)] = make_shared<ConstantBuffer>();
	_constantBuffer[static_cast<uint8>(CBV_REGISTER::b0)]->Init(CBV_REGISTER::b0, sizeof(CameraData), 256);

	_constantBuffer[static_cast<uint8>(CBV_REGISTER::b1)] = make_shared<ConstantBuffer>();
	_constantBuffer[static_cast<uint8>(CBV_REGISTER::b1)]->Init(CBV_REGISTER::b1, sizeof(TransformData), 256);

}

void Graphics::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Graphics::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Graphics::SetViewport()
{
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;
	_viewport.Width = static_cast<float>(800);
	_viewport.Height = static_cast<float>(600);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;

	_scissorRect = CD3DX12_RECT(0, 0, 800, 600);
}


