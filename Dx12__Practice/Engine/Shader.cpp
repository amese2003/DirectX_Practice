#include "pch.h"
#include "Shader.h"
#include "VertexData.h"
#include "RenderStates.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::Init(const wstring& path, ShaderInfo info)
{
	CreateVertexShader(path, "VS_Main", "vs_5_0");
	CreatePixelShader(path, "PS_Main", "ps_5_0");

	vector<D3D12_INPUT_ELEMENT_DESC> desc = MeshVertex::VertexDesc;

	//_pipelineDesc.InputLayout = { desc, _countof(desc)};
	_pipelineDesc.InputLayout = { desc.data(), static_cast<UINT>(desc.size())};
	_pipelineDesc.pRootSignature = ROOT_SIGNATURE.Get();

	_pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	

	_pipelineDesc.SampleMask = UINT_MAX;
	_pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_pipelineDesc.NumRenderTargets = 1;
	_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_pipelineDesc.SampleDesc.Count = 1;
	_pipelineDesc.DSVFormat = GRAPHICS->GetDepthStencilBuffer()->GetDSVFormat();





	switch (info.blendType)
	{
	case BLEND_TYPE::AlphaToCoverageBS:
		_pipelineDesc.BlendState.AlphaToCoverageEnable = true;
		_pipelineDesc.BlendState.IndependentBlendEnable = false;
		_pipelineDesc.BlendState.RenderTarget[0].BlendEnable = false;
		_pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;

	case BLEND_TYPE::NoRenderTargetWritesBS:
		_pipelineDesc.BlendState.AlphaToCoverageEnable = false;
		_pipelineDesc.BlendState.IndependentBlendEnable = false;

		_pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;
		_pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		_pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		_pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		_pipelineDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		_pipelineDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		_pipelineDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		_pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;

	case BLEND_TYPE::TransparentBS:
		_pipelineDesc.BlendState.AlphaToCoverageEnable = false;
		_pipelineDesc.BlendState.IndependentBlendEnable = false;

		_pipelineDesc.BlendState.RenderTarget[0].BlendEnable = false;
		_pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		_pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		_pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		_pipelineDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		_pipelineDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		_pipelineDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		_pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0;
		break;

	default:
		break;
	}

	switch (info.rasterizerType)
	{
	case RASTERIZER_TYPE::WireframeRS:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		_pipelineDesc.RasterizerState.FrontCounterClockwise = false;
		_pipelineDesc.RasterizerState.DepthClipEnable = true;
		break;
	case RASTERIZER_TYPE::NoCullRS:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		_pipelineDesc.RasterizerState.FrontCounterClockwise = false;
		_pipelineDesc.RasterizerState.DepthClipEnable = true;
		break;
	case RASTERIZER_TYPE::CullClockwiseRS:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		_pipelineDesc.RasterizerState.FrontCounterClockwise = true;
		_pipelineDesc.RasterizerState.DepthClipEnable = true;
		break;
	default:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		break;
	}

	switch (info.depthStencilType)
	{
	case DEPTH_STENCIL_TYPE::MarkMirrorDSS:
		_pipelineDesc.DepthStencilState.DepthEnable = true;
		_pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		_pipelineDesc.DepthStencilState.StencilEnable = true;
		_pipelineDesc.DepthStencilState.StencilReadMask = 0xff;
		_pipelineDesc.DepthStencilState.StencilWriteMask = 0xff;

		_pipelineDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
		_pipelineDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

		// We are not rendering backfacing polygons, so these settings do not matter.
		_pipelineDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
		_pipelineDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		break;

	case DEPTH_STENCIL_TYPE::DrawReflectionDSS:
		_pipelineDesc.DepthStencilState.DepthEnable = true;
		_pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		_pipelineDesc.DepthStencilState.StencilEnable = true;
		_pipelineDesc.DepthStencilState.StencilReadMask = 0xff;
		_pipelineDesc.DepthStencilState.StencilWriteMask = 0xff;

		_pipelineDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

		// We are not rendering backfacing polygons, so these settings do not matter.
		_pipelineDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
		break;

	case DEPTH_STENCIL_TYPE::NoDoubleBlendDSS:
		_pipelineDesc.DepthStencilState.DepthEnable = true;
		_pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		_pipelineDesc.DepthStencilState.StencilEnable = true;
		_pipelineDesc.DepthStencilState.StencilReadMask = 0xff;
		_pipelineDesc.DepthStencilState.StencilWriteMask = 0xff;

		_pipelineDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_INCR;
		_pipelineDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

		// We are not rendering backfacing polygons, so these settings do not matter.
		_pipelineDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		_pipelineDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_INCR;
		_pipelineDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
		break;
	default:
		break;
	}


	HRESULT hr =  DEVICE->CreateGraphicsPipelineState(&_pipelineDesc, IID_PPV_ARGS(&_pipelineState));
}



void Shader::Update()
{
	CMD_LIST->SetPipelineState(_pipelineState.Get());
}

void Shader::CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	uint32 compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	if (FAILED(::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, name.c_str(), version.c_str(), compileFlag, 0, &blob, &_errBlob)))
	{
		::MessageBoxA(nullptr, "Shader Create Failed !", nullptr, MB_OK);
	}

	shaderByteCode = { blob->GetBufferPointer(), blob->GetBufferSize() };
}

void Shader::CreateVertexShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, _vsBlob, _pipelineDesc.VS);
}

void Shader::CreatePixelShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, _psBlob, _pipelineDesc.PS);
}
