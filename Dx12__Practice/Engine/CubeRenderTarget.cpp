#include "pch.h"
#include "CubeRenderTarget.h"

CubeRenderTarget::CubeRenderTarget(ComPtr<ID3D12Device> device, UINT width, UINT height, DXGI_FORMAT format)
{
	_device = device;

	_widht = width;
	_height = height;
	_format = format;

	_viewport = { 0.f, 0.f, (float)width, (float)height, 0.f, 1.f };
	_scissorRect = { 0, 0, (int)width, (int)height };

	BuildResource();
}

void CubeRenderTarget::BuildDescriptors(CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuSrv, CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuSrv, CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuRtv[6])
{
	// Save references to the descriptors. 
	_cpuHandle = hCpuSrv;
	_gpuHandle = hGpuSrv;

	for (int i = 0; i < 6; ++i)
		_rtvHandle[i] = hCpuRtv[i];

	//  Create the descriptors
	BuildDescriptors();
}

void CubeRenderTarget::OnResize(UINT newWidth, UINT newHeight)
{
	if ((_widht != newWidth) || (_height != newHeight))
	{
		_widht = newWidth;
		_height = newHeight;

		BuildResource();

		// New resource, so we need new descriptors to that resource.
		BuildDescriptors();
	}
}

void CubeRenderTarget::BuildDescriptors()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = _format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = 1;
	srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;

	_device->CreateShaderResourceView(_cubeMap.Get(), &srvDesc, _cpuHandle);

	// Create RTV to each cube face.
	for (int i = 0; i < 6; ++i)
	{
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Format = _format;
		rtvDesc.Texture2DArray.MipSlice = 0;
		rtvDesc.Texture2DArray.PlaneSlice = 0;

		// Render target to ith element.
		rtvDesc.Texture2DArray.FirstArraySlice = i;

		// Only view one element of the array.
		rtvDesc.Texture2DArray.ArraySize = 1;

		// Create RTV to ith cubemap face.
		_device->CreateRenderTargetView(_cubeMap.Get(), &rtvDesc, _rtvHandle[i]);
	}
}

void CubeRenderTarget::BuildResource()
{
	// Note, compressed formats cannot be used for UAV.  We get error like:
	// ERROR: ID3D11Device::CreateTexture2D: The format (0x4d, BC3_UNORM) 
	// cannot be bound as an UnorderedAccessView, or cast to a format that
	// could be bound as an UnorderedAccessView.  Therefore this format 
	// does not support D3D11_BIND_UNORDERED_ACCESS.

	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = _widht;
	texDesc.Height = _height;
	texDesc.DepthOrArraySize = 6;
	texDesc.MipLevels = 1;
	texDesc.Format = _format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	auto heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	ThrowIfFailed(_device->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cubeMap)));
}
