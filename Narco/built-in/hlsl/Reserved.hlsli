SamplerState defaultSampler : register(s0);

cbuffer Constants : register(b0)
{
    float4x4 gWorld;
    float4x4 gView;
    float4x4 gProjection;
    
    uint4 gMaterialID; // material id
}