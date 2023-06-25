#include "pch.h"
#include "Shader.h"
#include "VertexData.h"
#include "RenderStates.h"

Shader::Shader() : Super(ResourceType::Shader)
{
}

Shader::~Shader()
{
}

void Shader::Init(const wstring& path, ShaderInfo info, ShaderArg args)
{
	_info = info;
	_args = args;

	if (_info.shaderType == SHADER_TYPE::COMPUTE)
		CreateComputeShader(path);
	else
		CreateGraphicShader(path);
}



void Shader::Update()
{
	//CMD_LIST->SetPipelineState(_pipelineState.Get());
}

void Shader::CreateGraphicShader(const wstring& path)
{
	ZeroMemory(&_pipelineDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	CreateVertexShader(path, _args.vs, "vs_5_0");
	CreatePixelShader(path, _args.ps, "ps_5_0");

	if (_args.hs.empty() == false)
		CreateHullShader(path, _args.hs, "hs_5_0");

	if (_args.ds.empty() == false)
		CreateDomainShader(path, _args.ds, "ds_5_0");

	if (_args.gs.empty() == false)
		CreateGeometryShader(path, _args.gs, "gs_5_0");


	vector<D3D12_INPUT_ELEMENT_DESC> desc = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 60, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

		//{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		//{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		//{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		//{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
		//{ "WCOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
	};

	//_pipelineDesc.InputLayout = { desc, _countof(desc)};
	_pipelineDesc.InputLayout = { desc.data(), static_cast<UINT>(desc.size()) };
	_pipelineDesc.pRootSignature = ROOT_SIGNATURE.Get();


	//_pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	//_pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//_pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//_pipelineDesc.SampleMask = UINT_MAX;
	//_pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	//_pipelineDesc.NumRenderTargets = 1;
	//_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//_pipelineDesc.SampleDesc.Count = 1;
	//_pipelineDesc.SampleDesc.Quality = 0;
	//_pipelineDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//_pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//_pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//_pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);


	//_pipelineDesc.SampleMask = UINT_MAX;

	//switch (_info.topologyType)
	//{
	//case TOPOLOGY_TYPE::POINT:
	//	_pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	//	_topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	//	break;

	//case TOPOLOGY_TYPE::PATCH:
	//	_pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	//	_topology = D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
	//	break;
	//case TOPOLOGY_TYPE::TRIANGLE:
	//case TOPOLOGY_TYPE::DEFAULT:
	//default:
	//	_pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//	_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//	break;
	//}



	//_pipelineDesc.NumRenderTargets = 1;
	//_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//_pipelineDesc.SampleDesc.Count = 1;
	//_pipelineDesc.DSVFormat = GRAPHICS->GetDepthStencilBuffer()->GetDSVFormat();


	//switch (_info.blendType)
	//{
	//case BLEND_TYPE::AlphaToCoverageBS:
	//	_pipelineDesc.BlendState.AlphaToCoverageEnable = true;
	//	_pipelineDesc.BlendState.IndependentBlendEnable = false;
	//	_pipelineDesc.BlendState.RenderTarget[0].BlendEnable = false;
	//	_pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//	break;

	//case BLEND_TYPE::NoRenderTargetWritesBS:
	//	_pipelineDesc.BlendState.AlphaToCoverageEnable = false;
	//	_pipelineDesc.BlendState.IndependentBlendEnable = false;

	//	_pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;
	//	_pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//	_pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	//	_pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	//	_pipelineDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	//	_pipelineDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	//	_pipelineDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	//	_pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//	break;

	//case BLEND_TYPE::TransparentBS:
	//	_pipelineDesc.BlendState.AlphaToCoverageEnable = false;
	//	_pipelineDesc.BlendState.IndependentBlendEnable = false;

	//	_pipelineDesc.BlendState.RenderTarget[0].BlendEnable = false;
	//	_pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	//	_pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	//	_pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	//	_pipelineDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	//	_pipelineDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	//	_pipelineDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	//	_pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0;
	//	break;

	//case BLEND_TYPE::TranseparencyBS:
	//	_pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;
	//	_pipelineDesc.BlendState.RenderTarget[0].LogicOpEnable = false;
	//	_pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//	_pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	//	_pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	//	_pipelineDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	//	_pipelineDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	//	_pipelineDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	//	_pipelineDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	//	_pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//	break;
	//default:
	//	break;
	//}

	//switch (_info.rasterizerType)
	//{
	//case RASTERIZER_TYPE::WireframeRS:
	//	_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	//	_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	//	_pipelineDesc.RasterizerState.FrontCounterClockwise = false;
	//	_pipelineDesc.RasterizerState.DepthClipEnable = true;
	//	break;
	//case RASTERIZER_TYPE::NoCullRS:
	//	_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//	_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//	_pipelineDesc.RasterizerState.FrontCounterClockwise = false;
	//	_pipelineDesc.RasterizerState.DepthClipEnable = true;
	//	break;
	//case RASTERIZER_TYPE::CullClockwiseRS:
	//	_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//	_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	//	_pipelineDesc.RasterizerState.FrontCounterClockwise = true;
	//	_pipelineDesc.RasterizerState.DepthClipEnable = true;
	//	break;
	//case RASTERIZER_TYPE::CULL_NONE:
	//	_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//	break;
	//case RASTERIZER_TYPE::WireframeOnly:
	//	_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	//default:
	//	break;
	//}

	//switch (_info.depthStencilType)
	//{
	//case DEPTH_STENCIL_TYPE::MarkMirrorDSS:
	//	_pipelineDesc.DepthStencilState.DepthEnable = true;
	//	_pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//	_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	//	_pipelineDesc.DepthStencilState.StencilEnable = true;
	//	_pipelineDesc.DepthStencilState.StencilReadMask = 0xff;
	//	_pipelineDesc.DepthStencilState.StencilWriteMask = 0xff;

	//	_pipelineDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
	//	_pipelineDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	//	// We are not rendering backfacing polygons, so these settings do not matter.
	//	_pipelineDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
	//	_pipelineDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//	break;

	//case DEPTH_STENCIL_TYPE::DrawReflectionDSS:
	//	_pipelineDesc.DepthStencilState.DepthEnable = true;
	//	_pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//	_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	//	_pipelineDesc.DepthStencilState.StencilEnable = true;
	//	_pipelineDesc.DepthStencilState.StencilReadMask = 0xff;
	//	_pipelineDesc.DepthStencilState.StencilWriteMask = 0xff;

	//	_pipelineDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

	//	// We are not rendering backfacing polygons, so these settings do not matter.
	//	_pipelineDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
	//	break;

	//case DEPTH_STENCIL_TYPE::NoDoubleBlendDSS:
	//	_pipelineDesc.DepthStencilState.DepthEnable = true;
	//	_pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//	_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	//	_pipelineDesc.DepthStencilState.StencilEnable = true;
	//	_pipelineDesc.DepthStencilState.StencilReadMask = 0xff;
	//	_pipelineDesc.DepthStencilState.StencilWriteMask = 0xff;

	//	_pipelineDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_INCR;
	//	_pipelineDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

	//	// We are not rendering backfacing polygons, so these settings do not matter.
	//	_pipelineDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	//	_pipelineDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_INCR;
	//	_pipelineDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
	//	break;
	//default:
	//	break;
	//}



	_pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//_pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	_pipelineDesc.SampleMask = UINT_MAX;
	_pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_pipelineDesc.NumRenderTargets = 1;
	_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_pipelineDesc.SampleDesc.Count = 1;
	_pipelineDesc.SampleDesc.Quality = 00;
	//_pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;


	HRESULT hr = DEVICE->CreateGraphicsPipelineState(&_pipelineDesc, IID_PPV_ARGS(&_pipelineState));
	CHECK(hr);
}

void Shader::CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	uint32 compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	const D3D_SHADER_MACRO alphaTestDefines[] =
	{
		"FOG", "1",
		"ALPHA_TEST", "1",
		NULL, NULL
	};

	if (FAILED(::D3DCompileFromFile(path.c_str(), alphaTestDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE
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

void Shader::CreateHullShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, _hsBlob, _pipelineDesc.HS);
}

void Shader::CreateDomainShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, _dsBlob, _pipelineDesc.DS);
}

void Shader::CreateGeometryShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, _gsBlob, _pipelineDesc.GS);
}

void Shader::CreatePixelShader(const wstring& path, const string& name, const string& version)
{
	CreateShader(path, name, version, _psBlob, _pipelineDesc.PS);
}



void Shader::CreateComputeShader(const wstring& path)
{
	/*_info.shaderType = SHADER_TYPE::COMPUTE;

	_computePipelineDesc.pRootSignature = COMPUTE_ROOT_SIGNATURE.Get();


	CreateShader(path, _args.cs, "cs_5_0", _csBlob, _computePipelineDesc.CS);
	_computePipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	HRESULT hr = DEVICE->CreateComputePipelineState(&_computePipelineDesc, IID_PPV_ARGS(&_pipelineState));
	CHECK(hr);*/
}
