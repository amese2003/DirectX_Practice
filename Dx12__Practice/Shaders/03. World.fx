
cbuffer CameramData : register(b0)
{
    row_major matrix matView;
    row_major matrix matProjection;
};

cbuffer TransformData : register(b1)
{
    row_major matrix matWorld;
};

struct VS_IN
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    output.pos = float4(input.pos, 1.f);
    output.color = input.color;

    return output;
    /*VS_OUT output = (VS_OUT)0;

    output.pos = float4(input.pos, 1.f);
    output.pos = mul(input.pos, matWorld);
    output.pos = mul(input.pos, matView);
    output.pos = mul(input.pos, matProjection);

    output.color = input.color;

    return output;*/
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    return input.color;
}