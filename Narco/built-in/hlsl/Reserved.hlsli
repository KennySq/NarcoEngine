SamplerState defaultSampler : register(s0);

cbuffer Constants : register(b0)
{
    float4x4 gWorld;
    float4x4 gView;
    float4x4 gProjection;
}

struct PointLight
{
    
};

struct DirectionalLight
{
    
};

StructuredBuffer<PointLight> PointLightBuffer : register(t0);
StructuredBuffer<DirectionalLight> DirectionalLightBuffer : register(t1);