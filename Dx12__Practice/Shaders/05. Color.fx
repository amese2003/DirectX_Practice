#include "00. Common.fx"



VertexColorOutput VS_Main(VertexTextureNormalTangent input)
{
    VertexColorOutput output = (VertexColorOutput)0;

    output.position = float4(input.position, 1.f);
    /*output.position = mul(output.position, gWorld);
    output.position = mul(output.position, gView);
    output.position = mul(output.position, gProjection);*/

    output.position = mul(output.position, gWorld);
    output.position = mul(output.position, gViewProj);
    output.color = input.color;
    return output;
}

float4 PS_Main(VertexColorOutput input) : SV_Target
{
    return input.color;
}