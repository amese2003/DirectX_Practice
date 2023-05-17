

#ifndef _GLOBAL_FX_
#define _GLOBAL_FX_



////////////////
// VertexData //
////////////////

struct Vertex
{
	float4 position : POSITION;
};

struct VertexTexture
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VertexColor
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct VertexTextureNormal
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VertexTextureNormalTangent
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 color : COLOR;
};

struct VertexTextureNormalTangentBlend
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 blendIndices : BLEND_INDICES;
	float4 blendWeights : BLEND_WEIGHTS;
};

//////////////////
// VertexOutput //
//////////////////

struct VertexOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct MeshOutput
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION1;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct VertexColorOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

cbuffer TransformData : register(b1)
{
	float4		     position;
	row_major matrix gWorld;
	row_major matrix gView;
	row_major matrix gProjection;
	row_major matrix gWorldInvTranspose;
	row_major matrix gWorldViewProj;
	row_major matrix gTexTransform;
};


cbuffer MaterialData : register(b2)
{
	Material gMaterial;
}

#endif