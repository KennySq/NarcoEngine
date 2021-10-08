struct PointLight
{
    float3 mPosition;
    float mIntensity;
    
    float4 mColor;
};

struct DirectionalLight
{
    float3 mDirection;
    float mIntensity;
    
    float4 mColor;
};

StructuredBuffer<PointLight> PointLightBuffer : register(t0);
StructuredBuffer<DirectionalLight> DirectionalLightBuffer : register(t1);