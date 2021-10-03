SamplerState defaultSampler;

Texture2D<float4> gPosition : register(t0);
Texture2D<float4> gWorldPosition : register(t1);
Texture2D<float4> gNormal : register(t2);
Texture2D<float4> gWorldNormal : register(t3);
Texture2D<float2> gTexcoord : register(t4);
Texture2D<float4> gAlbedo : register(t5);


/*	float4 mProjection : SV_Target0;
	float4 mWorldPosition : SV_Target1;
	float4 mNormal : SV_Target2;
	float4 mWorldNormal : SV_Target3;
	float2 mTexcoord : SV_Target4;
    float4 mAlbedo : SV_Target5;*/
struct Vertex_Input
{
    float3 mPosition : POSITION0;
    float2 mTexcoord : TEXCOORD0;
};

struct Pixel_Input
{
    float4 mPosition : SV_Position;
    float2 mTexcoord : TEXCOORD0;
};

Pixel_Input vert(Vertex_Input input)
{
    Pixel_Input output = (Pixel_Input) 0;
    
    output.mPosition = float4(input.mPosition, 1.0f);
    output.mTexcoord = input.mTexcoord;
    
	return output;
}

float4 frag(Pixel_Input input) : SV_Target0
{
    float4 color = float4(0, 0, 0, 0);
    
    float4 position = gPosition.Sample(defaultSampler, input.mTexcoord);
    float4 normal = gNormal.Sample(defaultSampler, input.mTexcoord);
    float2 uv = gTexcoord.Sample(defaultSampler, input.mTexcoord);
    float4 worldPosition = gWorldPosition.Sample(defaultSampler, input.mTexcoord);
    float4 worldNormal = gWorldNormal.Sample(defaultSampler, input.mTexcoord);
    float4 albedo = gAlbedo.Sample(defaultSampler, input.mTexcoord);
    
    return worldNormal;
}