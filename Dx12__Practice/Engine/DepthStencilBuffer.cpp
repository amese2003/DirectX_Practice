#include "pch.h"
#include "DepthStencilBuffer.h"

DepthStencilBuffer::DepthStencilBuffer()
{
}

DepthStencilBuffer::~DepthStencilBuffer()
{
}

void DepthStencilBuffer::Init()
{
	GRAPHICS->GetCmdQueue()->WaitSync();
	GRAPHICS->GetCmdQueue()->GetCmdList()->Reset(GRAPHICS->GetCmdQueue()->GetAlloc().Get(), nullptr);


	_texture = make_shared<Texture>();
	_texture->CreateTexture(GRAPHICS->GetBackBufferFormat(), GRAPHICS->GetViewport().Width, GRAPHICS->GetViewport().Height, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	RESOURCES->Add(L"depthStencil", _texture);

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(_texture->GetComPtr().Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);

	GRAPHICS->GetCmdQueue()->GetCmdList()->ResourceBarrier(1, &barrier);
	GRAPHICS->GetCmdQueue()->ExecuteCommandList();
}
