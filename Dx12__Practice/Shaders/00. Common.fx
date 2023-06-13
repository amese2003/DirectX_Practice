

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
	float2 sizeW : SIZE;
};

struct VertexTextureNormalTangentInstance
{
	float3				position : POSITION;
	float2				uv : TEXCOORD;
	float3				normal : NORMAL;
	float3				tangent : TANGENT;
	float4				color : COLOR;
	float2				sizeW : SIZE;

	// Instancing
	row_major matrix	World		: WORLD;
	float4				Color1		: COLOR1;
	uint				InstanceId	: SV_InstanceID;
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

struct LightInfo
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4      Emmissive;

	float3		Direction;
	float		Range;

	float3		Att;
	float		Spot;

	float3		Position;
	float		pad;
};

cbuffer GlobalLightData : register(b0)
{
	int		lightCount;
	float3	gEyePosW;
	LightInfo g_light[50];

	float gFogStart;
	float gFogRange;
	float2 pad;
	float4 gFogColor;
};



struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};


cbuffer TransformData : register(b1)
{
	float4		     position;
	row_major matrix gWorld;
	row_major matrix gView;
	row_major matrix gProjection;
	row_major matrix gWorldInvTranspose;
	row_major matrix gWorldViewProj;
	row_major matrix gViewProj;
};


cbuffer MaterialData : register(b2)
{
	Material gMaterial;
	row_major matrix gTexTransform;
	float3   gFresnelR0;
	float    gRoughness;
}


#endif