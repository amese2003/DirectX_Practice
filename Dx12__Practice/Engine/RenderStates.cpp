#include "pch.h"
#include "RenderStates.h"


D3D12_BLEND_DESC RenderStates::InitBlend(BLEND_TYPE blendState)
{
	D3D12_BLEND_DESC desc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	switch (blendState)
	{
	case BLEND_TYPE::AlphaToCoverageBS:
		desc.AlphaToCoverageEnable = true;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;
		
	case BLEND_TYPE::NonRenderTargetWritesBS:
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		break;

	case BLEND_TYPE::TransparentBS:
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;

		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = 0;
		break;

	default:
		break;
	}


	return desc;
}

D3D12_RASTERIZER_DESC RenderStates::InitRasterizer(RASTERIZER_TYPE rasterizeState)
{
	D3D12_RASTERIZER_DESC desc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);


	switch (rasterizeState)
	{
	case RASTERIZER_TYPE::WireframeRS:
		desc.FillMode = D3D12_FILL_MODE_WIREFRAME;
		break;
	default:
		desc.FillMode = D3D12_FILL_MODE_SOLID;
		break;
	}

	return desc;
}

D3D12_DEPTH_STENCIL_DESC RenderStates::InitDepthStencil(DEPTH_STENCIL_TYPE depthState)
{
	D3D12_DEPTH_STENCIL_DESC desc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	switch (depthState)
	{
	default:
		break;
	}

	return desc;
}
