SamplerState defaultSampler : register(s0);

cbuffer Constants : register(b0)
{
	float4x4 gWorld;
	float4x4 gView;
	float4x4 gProjection;
}

Texture2D<float4> gBaseMap;
Texture2D<float4> gNormalMap;

Buffer<float> gMetallic;
Buffer<float> gSmoothness;
Buffer<float2> gSomeFloat2;
Buffer<float3> gSomeFloat3;

struct Vertex_Input
{
	float3 mPosition : POSITION0;
	float3 mNormal : NORMAL0;
    float3 mBinormal : BINORMAL0;
    float3 mTangent : TANGENT0;
	float2 mTexcoord : TEXCOORD0;
};

struct Pixel_Input
{
	float4 mPosition : SV_Position;
	float4 mWorldPosition : TEXCOORD0;
	float4 mNormal : TEXCOORD1;
	float4 mWorldNormal : TEXCOORD2;
	float2 mTexcoord : TEXCOORD3;
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
	
    output.mPosition = mul(float4(input.mPosition, 1.0f), gWorld);
	output.mPosition = mul(output.mPosition, gView);
	output.mPosition = mul(output.mPosition, gProjection);
	
	output.mWorldNormal = float4(mul(input.mNormal, (float3x3) gWorld), 1.0f);
	output.mNormal = float4(input.mNormal, 1.0f);
	
	output.mTexcoord = input.mTexcoord;
	
	return output;
}

Buffer_Input frag(Pixel_Input input)
{
	Buffer_Input output = (Buffer_Input) 0;
	
	float4 albedo = gBaseMap.Sample(defaultSampler, input.mTexcoord);
    float4 normal = gNormalMap.Sample(defaultSampler, input.mTexcoord);
	
	output.mProjection = input.mPosition;
    output.mWorldPosition = input.mWorldPosition;
	
	output.mNormal = input.mNormal;
	output.mWorldNormal = input.mWorldNormal;
	output.mTexcoord = input.mTexcoord;
	
    output.mAlbedo = 0;
    output.mAlbedo = 0;
	
    output.mNormal = normal;
	
	return output;
};