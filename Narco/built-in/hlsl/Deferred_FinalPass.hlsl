SamplerState defaultSampler;

Texture2D gPosition;
Texture2D gNormal;
Texture2D gTexcoord;

struct Vertex_Input
{
    float4 mPosition : POSITION0;
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
    
    output.mPosition = input.mPosition;
    output.mTexcoord = input.mTexcoord;
    
	return output;
}

float4 frag(Pixel_Input input) : SV_Target0
{
    float4 color = float4(0, 0, 0, 0);
    
    float4 position = gPosition.Sample(defaultSampler, input.mTexcoord);
    float4 nomral = gNormal.Sample(defaultSampler, input.mTexcoord);
    float2 uv = gTexcoord.Sample(defaultSampler, input.mTexcoord);
    
    return position;

}