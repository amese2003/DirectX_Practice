#include "pch.h"
#include "BlurFilter.h"

BlurFilter::BlurFilter()
{
}

BlurFilter::~BlurFilter()
{
}

void BlurFilter::Init(UINT width, UINT height, DXGI_FORMAT format)
{
	mWidth = width;
	mHeight = height;
	mFormat = format;

	mCbvSrvUavDescriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_rootsignature = GRAPHICS->GetRootsignature()->GetComputeSignature();
	BuildDescriptorHeap();
	BuildResources();

	HRESULT hr = GRAPHICS->GetComputeQueue()->GetAlloc()->Reset();
	CHECK(hr);

	hr = GRAPHICS->GetComputeQueue()->GetCmdList()->Reset(GRAPHICS->GetComputeQueue()->GetAlloc().Get(), nullptr);
	CHECK(hr);

	hr =  GRAPHICS->GetComputeQueue()->GetCmdList()->Close();
	CHECK(hr);
	ID3D12CommandList* cmdsLists[] = { GRAPHICS->GetComputeQueue()->GetCmdList().Get() };
	GRAPHICS->GetComputeQueue()->GetCmdQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	GRAPHICS->GetComputeQueue()->WaitSync();
}

void BlurFilter::BuildDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = textureDescriptorCount +
		blurDescriptorCount;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap));

	_srvHandle = _srvHeap->GetCPUDescriptorHandleForHeapStart();
}

void BlurFilter::PushResource(ComPtr<ID3D12Resource> resource)
{
	if (_textureCount >= textureDescriptorCount)
		return;

	_textureCount++;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = resource->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	DEVICE->CreateShaderResourceView(resource.Get(), &srvDesc, _srvHandle);

	// next descriptor
	if (_textureCount < textureDescriptorCount)
		_srvHandle.Offset(1, mCbvSrvUavDescriptorSize);

}

void BlurFilter::CommitResource()
{
	BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE(_srvHeap->GetCPUDescriptorHandleForHeapStart(), 3, mCbvSrvUavDescriptorSize),
		CD3DX12_GPU_DESCRIPTOR_HANDLE(_srvHeap->GetGPUDescriptorHandleForHeapStart(), 3, mCbvSrvUavDescriptorSize),
		mCbvSrvUavDescriptorSize);

}

void BlurFilter::BuildDescriptors(CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDescriptor, CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuDescriptor, UINT descriptorSize)
{
	mBlur0CpuSrv = hCpuDescriptor;
	mBlur0CpuUav = hCpuDescriptor.Offset(1, descriptorSize);
	mBlur1CpuSrv = hCpuDescriptor.Offset(1, descriptorSize);
	mBlur1CpuUav = hCpuDescriptor.Offset(1, descriptorSize);

	mBlur0GpuSrv = hGpuDescriptor;
	mBlur0GpuUav = hGpuDescriptor.Offset(1, descriptorSize);
	mBlur1GpuSrv = hGpuDescriptor.Offset(1, descriptorSize);
	mBlur1GpuUav = hGpuDescriptor.Offset(1, descriptorSize);

	BuildDescriptors();
}



void BlurFilter::Update(UINT width, UINT height)
{
	if ((mWidth != width) || (mHeight != height))
	{
		mWidth = width;
		mHeight = height;

		BuildResources();

		// New resource, so we need new descriptors to that resource.
		BuildDescriptors();
	}
}

void BlurFilter::Execute(ComPtr<ID3D12Resource> input, int blurCount)
{
	auto cmdList = GRAPHICS->GetComputeQueue()->GetCmdList();
	auto cmdListAlloc = GRAPHICS->GetComputeQueue()->GetAlloc();

	HRESULT hr = cmdListAlloc->Reset();
	CHECK(hr);

	/*hr = cmdList->Reset(cmdListAlloc.Get(), GRAPHICS->GetComputeShader(0)->GetPipelineState().Get());
	CHECK(hr);*/
	 


	auto weights = CalcGaussWeights(2.5f);
	int blurRadius = (int)weights.size() / 2;

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	

	
	cmdList->SetComputeRootSignature(_rootsignature.Get());


	cmdList->SetComputeRoot32BitConstants(0, 1, &blurRadius, 0);
	cmdList->SetComputeRoot32BitConstants(0, (UINT)weights.size(), weights.data(), 1);

	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(input.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE);

	cmdList->ResourceBarrier(1, &barrier);
	
	barrier = CD3DX12_RESOURCE_BARRIER::Transition(_blurMap0.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);

	cmdList->ResourceBarrier(1, &barrier);

	// Copy the input (back-buffer in this example) to BlurMap0.
	cmdList->CopyResource(_blurMap0.Get(), input.Get());


	barrier = CD3DX12_RESOURCE_BARRIER::Transition(_blurMap0.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);

	cmdList->ResourceBarrier(1, &barrier);


	barrier = CD3DX12_RESOURCE_BARRIER::Transition(_blurMap1.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	cmdList->ResourceBarrier(1, &barrier);

	for (int i = 0; i < blurCount; ++i)
	{
		//
		// Horizontal Blur pass.
		//

		

		//cmdList->SetPipelineState(horzBlurPipeline.Get());



		BlurParam param;
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GRAPHICS->GetComputeConstantBuffer(CBV_REGISTER::b0)->PushData(&param, sizeof(BlurParam));
		GRAPHICS->GetComputeDescHeap()->SetCBV(handle, CBV_REGISTER::b0);
		GRAPHICS->GetComputeDescHeap()->CommitTable();
		cmdList->SetComputeRootDescriptorTable(1, mBlur0GpuSrv);
		cmdList->SetComputeRootDescriptorTable(2, mBlur1GpuUav);

		// How many groups do we need to dispatch to cover a row of pixels, where each
		// group covers 256 pixels (the 256 is defined in the ComputeShader).
		UINT numGroupsX = (UINT)ceilf(mWidth / 256.0f);
		cmdList->Dispatch(numGroupsX, mHeight, 1);

		 barrier = CD3DX12_RESOURCE_BARRIER::Transition(_blurMap0.Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

		cmdList->ResourceBarrier(1, &barrier);

		barrier = CD3DX12_RESOURCE_BARRIER::Transition(_blurMap1.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);

		cmdList->ResourceBarrier(1, &barrier);

		//
		// Vertical Blur pass.
		//

		//cmdList->SetPipelineState(vertBlurPipeline.Get());


		GRAPHICS->GetComputeConstantBuffer(CBV_REGISTER::b0)->PushData(&param, sizeof(BlurParam));
		GRAPHICS->GetComputeDescHeap()->SetCBV(handle, CBV_REGISTER::b0);
		GRAPHICS->GetComputeDescHeap()->CommitTable();
		cmdList->SetComputeRootDescriptorTable(1, mBlur1GpuSrv);
		cmdList->SetComputeRootDescriptorTable(2, mBlur0GpuUav);

		// How many groups do we need to dispatch to cover a column of pixels, where each
		// group covers 256 pixels  (the 256 is defined in the ComputeShader).
		UINT numGroupsY = (UINT)ceilf(mHeight / 256.0f);
		cmdList->Dispatch(mWidth, numGroupsY, 1);


		 barrier = CD3DX12_RESOURCE_BARRIER::Transition(_blurMap0.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ);

		cmdList->ResourceBarrier(1, &barrier);

		barrier = CD3DX12_RESOURCE_BARRIER::Transition(_blurMap1.Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

		cmdList->ResourceBarrier(1, &barrier);
	}




}

vector<float> BlurFilter::CalcGaussWeights(float sigma)
{
	float twoSigma2 = 2.0f * sigma * sigma;

	// Estimate the blur radius based on sigma since sigma controls the "width" of the bell curve.
	// For example, for sigma = 3, the width of the bell curve is 
	int blurRadius = (int)ceil(2.0f * sigma);

	blurRadius <= MaxBlurRadius;

	std::vector<float> weights;
	weights.resize(2 * blurRadius + 1);

	float weightSum = 0.0f;

	for (int i = -blurRadius; i <= blurRadius; ++i)
	{
		float x = (float)i;

		weights[i + blurRadius] = expf(-x * x / twoSigma2);

		weightSum += weights[i + blurRadius];
	}

	// Divide by the sum so all the weights add up to 1.0.
	for (int i = 0; i < weights.size(); ++i)
	{
		weights[i] /= weightSum;
	}

	return weights;
}

void BlurFilter::BuildDescriptors()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = mFormat;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

	uavDesc.Format = mFormat;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	DEVICE->CreateShaderResourceView(_blurMap0.Get(), &srvDesc, mBlur0CpuSrv);
	DEVICE->CreateUnorderedAccessView(_blurMap0.Get(), nullptr, &uavDesc, mBlur0CpuUav);

	DEVICE->CreateShaderResourceView(_blurMap1.Get(), &srvDesc, mBlur1CpuSrv);
	DEVICE->CreateUnorderedAccessView(_blurMap1.Get(), nullptr, &uavDesc, mBlur1CpuUav);
}

void BlurFilter::BuildResources()
{
	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = mFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	CD3DX12_HEAP_PROPERTIES property = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	DEVICE->CreateCommittedResource(
		&property,
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&_blurMap0));

	DEVICE->CreateCommittedResource(
		&property,
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&_blurMap1));
}
