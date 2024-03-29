#pragma once

enum class SHADER_TYPE : uint8
{
	FORWARD,
	COMPUTE,
};


enum class RASTERIZER_TYPE : uint8
{
	DEFAULT = 0,
	WireframeRS,
	NoCullRS,
	CullClockwiseRS,
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WireframeOnly
};

enum class BLEND_TYPE : uint8
{
	DEFAULT = 0,
	AlphaToCoverageBS,
	TransparentBS,
	NoRenderTargetWritesBS,
	TranseparencyBS,
};

enum class DEPTH_STENCIL_TYPE : uint8
{
	DEFAULT = 0,
	MarkMirrorDSS,
	DrawReflectionDSS,
	NoDoubleBlendDSS,
};

enum class TOPOLOGY_TYPE : uint8
{
	DEFAULT = 0,
	TRIANGLE,
	POINT,
	PATCH,
};

struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::DEFAULT;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::DEFAULT;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
	TOPOLOGY_TYPE topologyType = TOPOLOGY_TYPE::DEFAULT;
};

class RenderStates
{
public:
	static D3D12_BLEND_DESC InitBlend(BLEND_TYPE blendState);
	static D3D12_RASTERIZER_DESC InitRasterizer(RASTERIZER_TYPE rasterizeState);
	static D3D12_DEPTH_STENCIL_DESC InitDepthStencil(DEPTH_STENCIL_TYPE depthState);
};

