struct VSInput
{
    float3 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

VSOutput VSMain(in VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1.0f);
    
    return output;
}