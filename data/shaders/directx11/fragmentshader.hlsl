struct VSOutput
{
    float4 position : SV_POSITION;
};

cbuffer PSUniforms : register(b1)
{
    float4 u_Color;
};

float4 PSMain(in VSOutput input) : SV_TARGET
{
    return u_Color;
}