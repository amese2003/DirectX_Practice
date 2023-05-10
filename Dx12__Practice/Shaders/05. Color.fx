#include "00. Common.fx"

cbuffer TEST_B1 : register(b1)
{
    row_major matrix world;
    row_major matrix viewtest;
    row_major matrix protest;
};


VertexColorOutput VS_Main(VertexTextureNormalTangent input)
{
    VertexColorOutput output = (VertexColorOutput)0;

    output.position = float4(input.position, 1.f);
    output.position = mul(output.position, world);
    output.position = mul(output.position, viewtest);
    output.position = mul(output.position, protest);


    output.color = input.color;

    return output;
}

float4 PS_Main(VertexColorOutput input) : SV_Target
{
    

    return input.color;
}