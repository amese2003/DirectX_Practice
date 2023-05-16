#include "00. Common.fx"





MeshOutput VS_Main(VertexTextureNormalTangent input)
{
    MeshOutput output = (MeshOutput)0;

    output.position = float4(input.position, 1.f);
    output.position = mul(output.position, gWorld);
    //output.position = mul(output.position, view);
    //output.position = mul(output.position, projection);
    output.normal = input.normal;

    return output;
}

float4 PS_Main(MeshOutput input) : SV_Target
{
    return float4(input.normal, 1.f);
}