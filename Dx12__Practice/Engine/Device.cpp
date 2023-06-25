#include "pch.h"
#include "Device.h"

void Device::Init()
{
	// D3D12 ������� Ȱ��ȭ
	// - VC++ ���â�� ���� ����� �޽��� ���
	// - riid : ����̽��� COM ID
	// - ppDevice : ������ ��ġ�� �Ű������� ����
#ifdef _DEBUG
	::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
	_debugController->QueryInterface(IID_PPV_ARGS(&_debugControllerinterface));
	_debugController->EnableDebugLayer();
	_debugControllerinterface->SetEnableGPUBasedValidation(true);
#endif

	// DXGI(DirectX Graphics Infrastructure)
	// Direct3D�� �Բ� ���̴� API
	// - ��ü ȭ�� ��� ��ȯ
	// - �����Ǵ� ���÷��� ��� ���� ��
	// CreateDXGIFactory
	// - riid : ����̽��� COM ID
	// - ppDevice : ������ ��ġ�� �Ű������� ����
	::CreateDXGIFactory1(IID_PPV_ARGS(&_dxgi));

	// CreateDevice
	// - ���÷��� �����(�׷��� ī��)�� ��Ÿ���� ��ü
	// - pAdapter : nullptr �����ϸ� �ý��� �⺻ ���÷��� �����
	// - MinimumFeatureLevel : ���� ���α׷��� �䱸�ϴ� �ּ� ��� ���� (���ڴٸ� �ɷ�����)
	// - riid : ����̽��� COM ID
	// - ppDevice : ������ ��ġ�� �Ű������� ����
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));



	// MSAA
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevel;
	qualityLevel.Format = GRAPHICS->GetBackBufferFormat();
	qualityLevel.SampleCount = 4;
	qualityLevel.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	qualityLevel.NumQualityLevels = 0;
	_device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &qualityLevel, sizeof(qualityLevel));

	GRAPHICS->Set4xMsaaLevel(qualityLevel.NumQualityLevels);
	CHECK(GRAPHICS->Get4xMsaaLevel() > 0 && "Unexpected MSAA quality level.");
}
