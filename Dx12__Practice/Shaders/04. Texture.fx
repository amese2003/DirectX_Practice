#include "00. Common.fx"

cbuffer TEST_B1 : register(b1)
{
    row_major matrix world;
};

Texture2D Texture0 : register(t0);
SamplerState Sampler0 : register(s0);

MeshOutput VS_Main(VertexTextureNormalTangent input)
{
    MeshOutput output = (MeshOutput)0;

    output.position = float4(input.position, 1.f);
    output.position = mul(output.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

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