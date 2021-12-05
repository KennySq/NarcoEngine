class PointLight
{
    float3 mPosition;
    float mIntensity;
    
    float4 mColor;
    
    float3 Diffuse(float3 normal)
    {
        return mPosition;
    }
    float3 Specular(float3 normal, float power)
    {
        return mPosition;
    }
};

class DirectionalLight
{
    float3 mDirection;
    float mIntensity;
    
    float4 mColor;
    
    float3 Diffuse(float3 normal)
    {
        return mDirection;

    }
    float3 Specular(float3 normal, float power)
    {
        return mDirection;

    }
};

// these resources are scene-level shared resources
cbuffer gPointLightBuffer : register(b1)
{
    PointLight gPointLights[2048];
}

cbuffer gDirectionalLightBuffer : register(b2)
{
    DirectionalLight gDirectionalLightInstances[1024];   
}