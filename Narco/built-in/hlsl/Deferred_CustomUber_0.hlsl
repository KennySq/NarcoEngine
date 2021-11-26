#include"Reserved.hlsli"

Texture2D<float4> gBaseMap : register(t0);
Texture2D<float4> gNormalMap : register(t1);

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

struct Buffer_Input
{
    float4 mProjection : SV_Target0;
    float4 mWorldPosition : SV_Target1;
    float4 mNormal : SV_Target2;
    float4 mWorldNormal : SV_Target3;
    float2 mTexcoord : SV_Target4;
    float4 mAlbedo : SV_Target5;
};


Pixel_Input vert(Vertex_Input input)
{
    Pixel_Input output = (Pixel_Input) 0;
	
    if (input.mMaterialID != gMaterialID.x)
    {
        return output;
    }
	
    float4 pos = float4(input.mPosition.xyz, 1.0f);
	
    output.mPosition = mul(pos.xyzw, gWorld);

    output.mWorldPosition = output.mPosition;
    output.mPosition = mul(output.mPosition, gView);
    output.mPosition = mul(output.mPosition, gProjection);
	
    output.mWorldNormal = float4(mul(input.mNormal, (float3x3) gWorld), 1.0f);
    output.mNormal = float4(input.mNormal, 1.0f);
	
 //   output.mTexcoord = float4(mul(input.mTexcoord, (float2x2) gWorld), 1.0f, 1.0f);
    output.mTexcoord = input.mTexcoord.xy;
	
    output.mMaterialID = input.mMaterialID;
	

	
	
    return output;
}

Buffer_Input frag(Pixel_Input input)
{
    Buffer_Input output = (Buffer_Input) 0;
	
    float2 uv = input.mTexcoord.xy;
	
    float4 albedo = gBaseMap.Sample(defaultSampler, uv);
    float4 normal = gNormalMap.Sample(defaultSampler, uv);
	
    output.mProjection = input.mPosition;
    output.mWorldPosition = input.mWorldPosition;
	
    output.mNormal = input.mNormal;
    output.mWorldNormal = input.mWorldNormal;
    output.mTexcoord = input.mTexcoord.xy;

    output.mAlbedo = float4(1, 0, 0, 0);
    
    //output.mNormal = normal;
	
    return output;
};