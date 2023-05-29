#include "pch.h"
#include "ComputeCommandQueue.h"

ComputeCommandQueue::ComputeCommandQueue()
{
}

ComputeCommandQueue::~ComputeCommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void ComputeCommandQueue::Init(ComPtr<ID3D12Device> device)
{
	D3D12_COMMAND_QUEUE_DESC computeQueueDesc = {};
	computeQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	computeQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	HRESULT hr;
	
	hr = device->CreateCommandQueue(&computeQueueDesc, IID_PPV_ARGS(&_cmdQueue));
	CHECK(hr);

	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));
	CHECK(hr);
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));
	CHECK(hr);

	hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	// CreateFence
	// - CPU와 GPU의 동기화 수단으로 쓰인다
	hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	CHECK(hr);
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	hr = _cmdList->Close();
	CHECK(hr);
}

void ComputeCommandQueue::WaitSync()
{
	_fenceValue++;

	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	if (_fence->GetCompletedValue() < _fenceValue)
	{
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}

void ComputeCommandQueue::FlushComputeCommandQueue()
{

	HRESULT hr;
	hr = _cmdList->Close();
	CHECK(hr);

	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	auto t = _countof(cmdListArr);
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	

	hr = _cmdAlloc->Reset();
	CHECK(hr);
	hr = _cmdList->Reset(_cmdAlloc.Get(), nullptr);
	CHECK(hr);

	COMPUTE_CMD_LIST->SetComputeRootSignature(GRAPHICS->GetRootsignature()->GetComputeSignature().Get());
}
\
