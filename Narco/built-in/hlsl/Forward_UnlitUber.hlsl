#include"Reserved.hlsli"

struct Vertex_Input
{
    float3 mPosition : POSITION0;
    float3 mNormal : NORMAL0;
    float3 mBinormal : BINORMAL0;
    float3 mTangent : TANGENT0;
    float2 mTexcoord : TEXCOORD0;
    uint mMaterialID : TEXCOORD1;
};

struct Pixel_Input
{
    float4 mPosition : SV_Position;
    float4 mWorldPosition : TEXCOORD0;
    float4 mNormal : TEXCOORD1;
    float4 mWorldNormal : TEXCOORD2;
    float2 mTexcoord : TEXCOORD3;
    uint mMaterialID : TEXCOORD4;
};

Pixel_Input vert(Vertex_Input input)
{
    Pixel_Input output = (Pixel_Input) 0;
	
    if (input.mMaterialID != gMaterialID.x)
    {
        return output;
    }
	
    float4 pos = float4(input.mPosition.xyz, 1.0f);
	
    output.mPosition = mul(pos, gWorld);

    output.mWorldPosition = output.mPosition;
    output.mPosition = mul(output.mPosition, gView);
    output.mPosition = mul(output.mPosition, gProjection);
	
    output.mWorldNormal = float4(mul(input.mNormal, (float3x3) gWorld), 1.0f);
    output.mNormal = float4(input.mNormal, 1.0f);
	
    output.mTexcoord = input.mTexcoord.xy;
	
    output.mMaterialID = input.mMaterialID;
	
    return output;
}

float4 frag(Pixel_Input input) : SV_Target0
{
    return input.mPosition;
}