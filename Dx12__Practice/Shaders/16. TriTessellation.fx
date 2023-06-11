
struct VertexTextureNormalTangent
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 color : COLOR;
	float2 sizeW : SIZE;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;

	float4 DiffuseAlbedo;
	float3 FresnelR0;
	float Shininess;
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
Texture2D gDiffuseMap : register(t0);
SamplerState samAnisotropic : register(s0)
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};


struct VertexOut
{
	float3 PosL    : POSITION;
};

// In Tessellation pipeline, VS is mostly used for animation to animate a coarse mesh
// before tessellating it.
VertexOut VS_Main(VertexTextureNormalTangent vin)
{
	VertexOut vout;

	vout.PosL = vin.position;

	return vout;
}

struct PatchTess
{
	float EdgeTess[3] : SV_TessFactor;
	float InsideTess : SV_InsideTessFactor;
};

// Generate and output per-patch data such as the quad tessellation factors.
// if a tessellation factor for a patch is set to zero, the patch is culled from the rest of the pipeline.
PatchTess ConstantHS(InputPatch<VertexOut, 3> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	pt.EdgeTess[0] = 6;
	pt.EdgeTess[1] = 12;
	pt.EdgeTess[2] = 3;

	pt.InsideTess = 1;
	/*
		float3 centerL = 0.25f*(patch[0].PosL + patch[1].PosL + patch[2].PosL);
		float3 centerW = mul(float4(centerL, 1.0f), gWorld).xyz;

		float d = distance(centerW, gEyePosW);

		// Tessellate the patch based on distance from the eye such that
		// the tessellation is 0 if d >= d1 and 60 if d <= d0.  The interval
		// [d0, d1] defines the range we tessellate in.

		const float d0 = 20.0f;
		const float d1 = 300.0f;
		float tess = 64.0f*saturate( (d1-d)/(d1-d0) );

		// Uniformly tessellate the patch.

		pt.EdgeTess[0] = tess;
		pt.EdgeTess[1] = tess;
		pt.EdgeTess[2] = tess;
		pt.EdgeTess[3] = tess;

		pt.InsideTess[0] = tess;
		pt.InsideTess[1] = tess;
		*/
	return pt;
}

struct HullOut
{
	float3 PosL : POSITION;
};

// This Hull Shader part is commonly used for a coordinate basis change, 
// for example changing from a quad to a Bezier bi-cubic.
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS_Main(InputPatch<VertexOut, 3> p,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
	HullOut hout;

	hout.PosL = p[i].PosL;

	return hout;
}

struct DomainOut
{
	float4 PosH : SV_POSITION;
};

// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("tri")]
DomainOut DS_Main(PatchTess patchTess,
	float3 baryCoords : SV_DomainLocation,
	const OutputPatch<HullOut, 3> tri)
{
	DomainOut dout;

	float3 p = baryCoords.x * tri[0].PosL + baryCoords.y * tri[1].PosL + baryCoords.z * tri[2].PosL;

	// Displacement mapping
	p.y = 0;// 0.3f*( p.z*sin(p.x) + p.x*cos(p.z) );

	dout.PosH = mul(float4(p, 1.0f), gWorldViewProj);

	return dout;
}

float4 PS_Main(DomainOut pin) : SV_Target
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}