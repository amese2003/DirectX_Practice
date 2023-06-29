#include "pch.h"
#include "Graphics.h"
#include "DepthStencilBuffer.h"

void Graphics::Init(HWND hwnd)
{


	
}

void Graphics::RenderBegin()
{

}

void Graphics::RenderEnd()
{

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

void Graphics::LogAdapters()
{
	UINT i = 0;
	ComPtr<IDXGIAdapter> adapter;

	vector<ComPtr<IDXGIAdapter>> adapterList;

	while (_device->GetDXGI()->EnumAdapters(i, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}
}

void Graphics::LogAdapterOutputs(ComPtr<IDXGIAdapter> adapter)
{
	UINT i = 0;
	ComPtr<IDXGIOutput> output = nullptr;

	while (adapter->EnumOutputs(i, output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";
		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, _backBufferFormat);
		++i;
	}

}

void Graphics::LogOutputDisplayModes(ComPtr<IDXGIOutput> output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugString(text.c_str());
	}
}


