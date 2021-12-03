
interface Light
{
    float3 Diffuse(float3 normal);
    float3 Specular(float3 normal, float power);
};

interface Material
{
    float GetMetallic();
    float GetRoughness();
    float3 GetSpecular();
  
    float3 GetAlbedo(float2 uv);
};

class PointLight : Light
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

class DirectionalLight : Light
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

cbuffer gLightBuffer : register(b1)
{
    PointLight gPointLightInstances[1024];
    DirectionalLight gDirectionalLightInstances[1024];
}