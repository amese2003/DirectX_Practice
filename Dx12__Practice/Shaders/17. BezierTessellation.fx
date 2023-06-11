
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
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

// Generate and output per-patch data such as the quad tessellation factors.
// if a tessellation factor for a patch is set to zero, the patch is culled from the rest of the pipeline.
PatchTess ConstantHS(InputPatch<VertexOut, 16> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	// Uniform tessellation for this demo.

	pt.EdgeTess[0] = 25;
	pt.EdgeTess[1] = 25;
	pt.EdgeTess[2] = 25;
	pt.EdgeTess[3] = 25;

	pt.InsideTess[0] = 25;
	pt.InsideTess[1] = 25;
	return pt;
}

struct HullOut
{
	float3 PosL : POSITION;
};

// This Hull Shader part is commonly used for a coordinate basis change, 
// for example changing from a quad to a Bezier bi-cubic.
[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(16)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS_Main(InputPatch<VertexOut, 16> p,
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

float4 BernsteinBasis(float t)
{
	float invT = 1.0f - t;

	return float4(invT * invT * invT,
		3.0f * t * invT * invT,
		3.0f * t * t * invT,
		t * t * t);
}

float3 CubicBezierSum(const OutputPatch<HullOut, 16> bezpatch, float4 basisU, float4 basisV)
{
	float3 sum = float3(0.0f, 0.0f, 0.0f);
	sum = basisV.x * (basisU.x * bezpatch[0].PosL + basisU.y * bezpatch[1].PosL + basisU.z * bezpatch[2].PosL + basisU.w * bezpatch[3].PosL);
	sum += basisV.y * (basisU.x * bezpatch[4].PosL + basisU.y * bezpatch[5].PosL + basisU.z * bezpatch[6].PosL + basisU.w * bezpatch[7].PosL);
	sum += basisV.z * (basisU.x * bezpatch[8].PosL + basisU.y * bezpatch[9].PosL + basisU.z * bezpatch[10].PosL + basisU.w * bezpatch[11].PosL);
	sum += basisV.w * (basisU.x * bezpatch[12].PosL + basisU.y * bezpatch[13].PosL + basisU.z * bezpatch[14].PosL + basisU.w * bezpatch[15].PosL);

	return sum;
}

float4 dBernsteinBasis(float t)
{
	float invT = 1.0f - t;

	return float4(-3 * invT * invT,
		3 * invT * invT - 6 * t * invT,
		6 * t * invT - 3 * t * t,
		3 * t * t);
}

// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("quad")]
DomainOut DS_Main(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 16> bezPatch)
{
	DomainOut dout;

	float4 basisU = BernsteinBasis(uv.x);
	float4 basisV = BernsteinBasis(uv.y);

	float3 p = CubicBezierSum(bezPatch, basisU, basisV);

	dout.PosH = mul(float4(p, 1.0f), gWorldViewProj);

	return dout;
}

float4 PS_Main(DomainOut pin) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
