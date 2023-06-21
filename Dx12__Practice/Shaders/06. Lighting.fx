#include "00. Common.fx"
#include "00. LightHelper.fx"





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
	output.NormalW = mul(input.normal, (float3x3)gWorld);

    // Transform to homogeneous clip space.
	output.PosH = mul(float4(output.PosW, 1.0f), gViewProj);

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

	ComputeDirectionalLight(gMaterial, g_light[0], input.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(gMaterial, g_light[1], input.PosW, input.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputeSpotLight(gMaterial, g_light[2], input.PosW, input.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	float4 litColor = ambient + diffuse + spec;

	//float4 litColor = ambient;
	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.Diffuse.a;

	return litColor;
}