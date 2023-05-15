#include "00. Common.fx"
#include "00. LightHelper.fx"


cbuffer TEST_B1 : register(b1)
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
	row_major matrix worldnvTranspose;
	row_major matrix worldViewProj;
};

cbuffer MaterialData : register(b2)
{
	Material gMaterial;
}

struct VertexOut
{
    float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
};


VertexOut VS_Main(VertexTextureNormalTangent input)
{
    VertexOut output = (VertexOut)0;

    // Transform to world space space.
	output.PosW = mul(float4(input.position, 1.0f), gWorld).xyz;
	output.NormalW = mul(input.normal, (float3x3)gWorldInvTranspose);

    // Transform to homogeneous clip space.
	output.PosH = mul(float4(input.position, 1.0f), gWorldViewProj);

    return output;
}

float4 PS_Main(VertexOut input) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	input.NormalW = normalize(input.NormalW);

	float3 toEyeW = normalize(gEyePosW - input.PosW);

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	float4 A, D, S;

	ComputeDirectionalLight(gMaterial, gDirLight, input.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(gMaterial, gPointLight, input.PosW, input.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputeSpotLight(gMaterial, gSpotLight, input.PosW, input.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	float4 litColor = ambient + diffuse + spec;

	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.Diffuse.a;

	return litColor;
}