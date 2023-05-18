#pragma once


struct ShaderInfo
{
	RASTERIZER_TYPE rasterizertype = RASTERIZER_TYPE::CullClockwiseRS;
	BLEND_TYPE blendtype = BLEND_TYPE::AlphaToCoverageBS;
	
};

enum RASTERIZER_TYPE
{
	Default = 0,
	WireframeRS,
	NoCullRS,
	CullClockwiseRS,
	CULL_BACK,
	CULL_FRONT,
};

enum BLEND_TYPE
{
	Default = 0,
	AlphaToCoverageBS,
	TransparentBS,
	NonRenderTargetWritesBS,
};

enum DEPTH_STENCIL_TYPE
{
	Default = 0,
	MarkMirrorDSS,
	DrawReflectionDSS,
	NoDoubleBlendDSS,
};

class RenderStates
{
	static D3D12_BLEND_DESC InitBlend(BLEND_TYPE blendState);
	static D3D12_RASTERIZER_DESC InitRasterizer(RASTERIZER_TYPE rasterizeState);
	static D3D12_DEPTH_STENCIL_DESC InitDepthStencil(DEPTH_STENCIL_TYPE depthState);
};

