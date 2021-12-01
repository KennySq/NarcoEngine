
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
    
    float3 Diffuse(float3 normal);
    float3 Specular(float3 normal, float power);
};

class DirectionalLight : Light
{
    float3 mDirection;
    float mIntensity;
    
    float4 mColor;
    
    float3 Diffuse(float3 normal);
    float3 Specular(float3 normal, float power);
};

Light gDirectionalLightInterface;
Light gPointLightInterface;
//Material gMaterialInterface;

cbuffer LightConstantsSlot0 : register(b1)
{
    PointLight gPointLight;
    DirectionalLight gDirectionalLight;
}