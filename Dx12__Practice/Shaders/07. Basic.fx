#include "00. Common.fx"
#include "00. LightHelper.fx"


Texture2D gDiffuseMap : register(t0);
SamplerState samAnisotropic : register(s0)
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = MIRROR;
	AddressV = MIRROR;
};


struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD;
};

VertexOut VS_Main(VertexTextureNormalTangent vin)
{
	VertexOut vout = (VertexOut)0;

	// Transform to world space space.
	vout.PosW = mul(float4(vin.position, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.normal, (float3x3)gWorldInvTranspose);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.position, 1.0f), gWorldViewProj);

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.uv, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}



float4 PS_Main(VertexOut pin) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;

	// Default to multiplicative identity.
	float4 texColor = float4(1, 1, 1, 1);
	//if (gUseTexure)
	//{
	//	// Sample texture.
	//	texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
	//}

	texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
	//float4 litColor = float4(pin.Tex, 0.0f, 1.0f);
	//
	//
	// Lighting.
	//

	float4 litColor = texColor;
	if (lightCount > 0)
	{
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// Sum the light contribution from each light source.  
		[unroll]
		for (int i = 0; i < lightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(gMaterial, g_light[i], pin.NormalW, toEye,
				A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}

		// Modulate with late add.
		litColor = texColor * (ambient + diffuse) + spec;
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

	return litColor;
}