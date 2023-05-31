#include "pch.h"
#include "ComputeTest.h"
#include <fstream>
#include <comdef.h>


void ComputeTest::Start()
{
	_test.resize(4);
	HRESULT hr = GRAPHICS->GetComputeQueue()->GetCmdList()->Reset(GRAPHICS->GetComputeQueue()->GetAlloc().Get(), nullptr);
	CHECK(hr);

	BuildBuffers();


	GRAPHICS->GetComputeQueue()->GetCmdList()->Close();
	ID3D12CommandList* cmdsLists[] = { GRAPHICS->GetComputeQueue()->GetCmdList().Get() };
	GRAPHICS->GetComputeQueue()->GetCmdQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	GRAPHICS->GetComputeQueue()->WaitSync();

	Dispatch();
}

void ComputeTest::Update()
{
}

void ComputeTest::BuildBuffers()
{
	// Generate some data.
	std::vector<Data> dataA(NumDataElements);
	std::vector<Data> dataB(NumDataElements);
	for (int i = 0; i < NumDataElements; ++i)
	{
		dataA[i].v1 = XMFLOAT3(i, i, i);
		dataA[i].v2 = XMFLOAT2(i, 0);

		dataB[i].v1 = XMFLOAT3(-i, i, 0.0f);
		dataB[i].v2 = XMFLOAT2(0, -i);
	}

	UINT64 byteSize = dataA.size() * sizeof(Data);

	_test[0] = make_shared<Texture>();
	_test[1] = make_shared<Texture>();
	_test[2] = make_shared<Texture>();
	_test[3] = make_shared<Texture>();

	_test[0]->CreateComputeTexture(dataA.data(), byteSize);
	_test[1]->CreateComputeTexture(dataB.data(), byteSize);
	_test[2]->CreateUAVTexture(byteSize);
	_test[3]->CreateCopyTexture(byteSize);

	////// Create some buffers to be used as SRVs.
	//mInputBufferA = CreateDefaultBuffer(
	//	dataA.data(),
	//	byteSize,
	//	mInputUploadBufferA);

	//mInputBufferB = CreateDefaultBuffer(
	//	dataB.data(),
	//	byteSize,
	//	mInputUploadBufferB);

	//D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(byteSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	//HRESULT hr;
	//// Create the buffer that will be a UAV.
	//hr =DEVICE->CreateCommittedResource(
	//	&heapProperty,
	//	D3D12_HEAP_FLAG_NONE,
	//	&desc,
	//	D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
	//	nullptr,
	//	IID_PPV_ARGS(&mOutputBuffer));

	//CHECK(hr);
	//D3D12_HEAP_PROPERTIES heapProperty1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);
	//D3D12_RESOURCE_DESC desc1 = CD3DX12_RESOURCE_DESC::Buffer(byteSize);


	//hr  = DEVICE->CreateCommittedResource(
	//	&heapProperty1,
	//	D3D12_HEAP_FLAG_NONE,
	//	&desc1,
	//	D3D12_RESOURCE_STATE_COPY_DEST,
	//	nullptr,
	//	IID_PPV_ARGS(&mReadBackBuffer));
	//CHECK(hr);
}

void ComputeTest::Dispatch()
{
	auto ComputeQueue = GRAPHICS->GetComputeQueue();
	auto mCommandQueue = GRAPHICS->GetComputeQueue()->GetCmdQueue();
	auto mCommandList = GRAPHICS->GetComputeQueue()->GetCmdList();
	auto mDirectCmdListAlloc = GRAPHICS->GetComputeQueue()->GetAlloc();

	auto devicetest = GRAPHICS;

	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	HRESULT hr =  mDirectCmdListAlloc->Reset();
	CHECK(hr);

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	hr = mCommandList->Reset(mDirectCmdListAlloc.Get(), _shader->GetPipelineState().Get());
	CHECK(hr);

	mCommandList->SetComputeRootSignature(COMPUTE_ROOT_SIGNATURE.Get());

	/*mCommandList->SetComputeRootShaderResourceView(0, mInputBufferA->GetGPUVirtualAddress());
	mCommandList->SetComputeRootShaderResourceView(1, mInputBufferB->GetGPUVirtualAddress());
	mCommandList->SetComputeRootUnorderedAccessView(2, mOutputBuffer->GetGPUVirtualAddress());*/

	mCommandList->SetComputeRootShaderResourceView(0, _test[0]->GetComPtr()->GetGPUVirtualAddress());
	mCommandList->SetComputeRootShaderResourceView(1, _test[1]->GetComPtr()->GetGPUVirtualAddress());
	mCommandList->SetComputeRootUnorderedAccessView(2, _test[2]->GetComPtr()->GetGPUVirtualAddress());


	mCommandList->Dispatch(1, 1, 1);


	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_test[2]->GetComPtr().Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_COPY_SOURCE);

	// Schedule to copy the data to the default buffer to the readback buffer.
	mCommandList->ResourceBarrier(1, &barrier);

	mCommandList->CopyResource(_test[3]->GetComPtr().Get(), _test[2]->GetComPtr().Get());

	D3D12_RESOURCE_BARRIER rbarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_test[2]->GetComPtr().Get(),
		D3D12_RESOURCE_STATE_COPY_SOURCE,
		D3D12_RESOURCE_STATE_COMMON);

	mCommandList->ResourceBarrier(1, &rbarrier);
	mCommandList->Close();
	//ComputeQueue->FlushComputeCommandQueue();

	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	ComputeQueue->WaitSync();

	// Map the data so we can read it on CPU.
	Data* mappedData = nullptr;

	
	hr = (_test[3]->GetComPtr()->Map(0, nullptr, reinterpret_cast<void**>(&mappedData)));
	CHECK(hr);

	std::ofstream fout("results.txt");

	for (int i = 0; i < NumDataElements; ++i)
	{
		fout << "(" << mappedData[i].v1.x << ", " << mappedData[i].v1.y << ", " << mappedData[i].v1.z <<
			", " << mappedData[i].v2.x << ", " << mappedData[i].v2.y << ")" << std::endl;
	}

	_test[3]->GetComPtr()->Unmap(0, nullptr);

}

ComPtr<ID3D12Resource> ComputeTest::CreateDefaultBuffer(const void* initData, UINT64 byteSize, ComPtr<ID3D12Resource>& uploadBuffer)
{
	ComPtr<ID3D12Resource> defaultBuffer;

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);

	HRESULT hr;

	// Create the actual default buffer resource.
	hr = DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(defaultBuffer.GetAddressOf()));

	CHECK(hr);
	D3D12_HEAP_PROPERTIES heapProperty1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// In order to copy CPU memory data into our default buffer, we need to create
	// an intermediate upload heap. 
	hr = DEVICE->CreateCommittedResource(
		&heapProperty1,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(uploadBuffer.GetAddressOf()));
	CHECK(hr);

	// Describe the data we want to copy into the default buffer.
	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = initData;
	subResourceData.RowPitch = byteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);

	// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
	// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
	// the intermediate upload heap data will be copied to mBuffer.
	COMPUTE_CMD_LIST->ResourceBarrier(1, &barrier);
	UpdateSubresources<1>(COMPUTE_CMD_LIST.Get(), defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
	
	
	barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	COMPUTE_CMD_LIST->ResourceBarrier(1, &barrier);

	// Note: uploadBuffer has to be kept alive after the above function calls because
	// the command list has not been executed yet that performs the actual copy.
	// The caller can Release the uploadBuffer after it knows the copy has been executed.


	return defaultBuffer;
}
