#include "pch.h"
#include "CommandQueue.h"
#include "DepthStencilBuffer.h"


CommandQueue::CommandQueue()
{
}

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void CommandQueue::Init()
{
	DEVICE->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	DEVICE->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(_cmdQueue.GetAddressOf()));

	DEVICE->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_cmdAlloc.GetAddressOf()));
	DEVICE->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(_cmdList.GetAddressOf()));;
	_cmdList->Close();

	DEVICE->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_resourceCmdAlloc.GetAddressOf()));
	DEVICE->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resourceCmdAlloc.Get(), nullptr, IID_PPV_ARGS(_resourceCmdList.GetAddressOf()));;
	_resourceCmdList->Close();


	DEVICE->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

}

void CommandQueue::WaitSync()
{
	// Advance the fence value to mark commands up to this fence point.
	_fenceValue++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		// Fire event when GPU hits current fence.  
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}

void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	//HRESULT hr= _renderAlloc[GRAPHICS->GetCurframenum()]->Reset();
	//CHECK(hr);
	//_cmdList->Reset(_renderAlloc[GRAPHICS->GetCurframenum()].Get(), nullptr);



	//_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());


	//GRAPHICS->GetConstantBuffer(CBV_REGISTER::b1)->Clear();
	//GRAPHICS->GetConstantBuffer(CBV_REGISTER::b2)->Clear();
	//GRAPHICS->GetTableDescHeap()->Clear();



	//

	//// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	//_cmdList->RSSetViewports(1, vp);
	//_cmdList->RSSetScissorRects(1, rect);

	//D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
	//	_swapChain->GetBackRTVBuffer().Get(),
	//	D3D12_RESOURCE_STATE_PRESENT, // 화면 출력
	//	D3D12_RESOURCE_STATE_RENDER_TARGET); // 외주 결과물

	//_cmdList->ResourceBarrier(1, &barrier);

	//// Specify the buffers we are going to render to.
	//D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	//_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);


	///*D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = GRAPHICS->GetDepthStencilBuffer()->GetDSVCpuHandle();
	//_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);*/

	//ID3D12DescriptorHeap* descHeap = GRAPHICS->GetTableDescHeap()->GetDescriptorHeap().Get();
	//_cmdList->SetDescriptorHeaps(1, &descHeap);

	//_cmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
}

void CommandQueue::RenderEnd()
{
	//D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
	//	_swapChain->GetBackRTVBuffer().Get(),
	//	D3D12_RESOURCE_STATE_RENDER_TARGET, // 외주 결과물
	//	D3D12_RESOURCE_STATE_PRESENT); // 화면 출력

	//_cmdList->ResourceBarrier(1, &barrier);
	//HRESULT hr = _cmdList->Close();
	//CHECK(hr);

	//// 커맨드 리스트 수행
	//ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	//_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);


	//

	//_swapChain->Present();

	//// Wait until frame commands are complete.  This waiting is inefficient and is
	//// done for simplicity.  Later we will show how to organize our rendering code
	//// so we do not have to wait per frame.
	//WaitSync();

	//

	//_swapChain->SwapIndex();
}

void CommandQueue::FlushResourceCommandQueue()
{
	//_resourceCmdList->Close();

	//ID3D12CommandList* cmdListArr[] = { _resourceCmdList.Get() };
	//_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	//WaitSync();

	//_resourceCmdAlloc->Reset();
	//_resourceCmdList->Reset(_resourceCmdAlloc.Get(), nullptr);

	CHECK(_resourceCmdList->Close());
	ID3D12CommandList* cmdListArr[] = { _resourceCmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);
	WaitSync();
}

void CommandQueue::ExecuteCommandList()
{
	//_cmdList->Close();

	//ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	//_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	//WaitSync();

	//_renderAlloc[GRAPHICS->GetCurframenum()]->Reset();
	//_cmdList->Reset(_renderAlloc[GRAPHICS->GetCurframenum()].Get(), nullptr);

	CHECK(_cmdList->Close());
	ID3D12CommandList* cmdsLists[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	WaitSync();
}
