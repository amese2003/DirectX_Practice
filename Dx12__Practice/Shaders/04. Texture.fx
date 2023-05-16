#include "00. Common.fx"



Texture2D Texture0 : register(t0);
SamplerState Sampler0 : register(s0);

MeshOutput VS_Main(VertexTextureNormalTangent input)
{
    MeshOutput output = (MeshOutput)0;

    output.position = float4(input.position, 1.f);
    /*output.position = mul(output.position, gWorld);
    output.position = mul(output.position, gView);
    output.position = mul(output.position, gProjection);*/

    output.position = mul(output.position, gWorldViewProj);

    output.uv = input.uv;
    output.normal = input.normal;
    output.tangent = input.tangent;

    return output;
}

float4 PS_Main(MeshOutput input) : SV_Target
{
    /*float4 color = float4(1.f, 1.f, 1.f, 1.f);
    color = Texture0.Sample(Sampler0, input.uv);*/
    float4 color = Texture0.Sample(Sampler0, input.uv);

    /*if (!Texture0)
        discard;*/

    return color;
}