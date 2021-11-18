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

StructuredBuffer<Vertex_Input> ReadVertexBuffer : register(t0);
Buffer<uint> ReadTriangleBuffer : register(t1);

struct Edge 
{
    uint A; // From
    uint B; // To
};

struct Triangle
{
    uint i0, i1, i2;
};


//struct Ray
//{
//    float3 Origin;
//    float3 Direction;
    
//    float3 At(float t)
//    {
//        return Origin + Direction * t;
//    }
//};

cbuffer Constants : register(b0)
{
    float4x4 View;
    float4x4 Projection;
    
    float4 ViewPosition;
    float4 ViewDirection;
}

RWTexture2D<float4> mDebugBuffer : register(u0);
RWStructuredBuffer<Edge> EdgeStream : register(u1);

shared uint i0, i1, i2;

// DTid.x == triangleCount
[numthreads(1, 1, 1)]
void comp( uint3 DTid : SV_DispatchThreadID, uint3 Gid : SV_GroupID, uint3 GTid : SV_GroupThreadID, uint Gi : SV_GroupIndex)
{
    // 1280, 720
    float2 resolution = float2(1280, 720); // (resolution / DTid.xy)
    float2 uv = (DTid.xy + uint2(1, 1)) / resolution;
 /*
    		eye = XMVectorSet(-1, 0, 0, 0);
		up = XMVectorSet(0, 1, 0, 0);
		at = XMVectorSet(0, 0, 0, 0);
    */
    //float3 rayOrigin = float3(ViewPosition.xyz);
    //float3 rayDir = float3(ViewDirection.xyz) + float3(uv, 0.0f);
    
    //Ray r = { rayOrigin, rayDir };
    
    
    // edge 테이블 초기화
    uint indexCount;
    uint triangleCount;
    ReadTriangleBuffer.GetDimensions(indexCount);
    
    triangleCount = indexCount / 3;
    
    uint index = Gid.x * 3 + GTid.x;
    uint i0, i1, i2;
    
    if (GTid.x == 0)
    {
        i0 = index;
    }
    
    if (GTid.x == 1)
    {
        i1 = index;
    }
    if (GTid.x == 2)
    {
        i2 == index;
    }
    
    GroupMemoryBarrierWithGroupSync();
    
    if (GTid.x == 0)
    {
        Edge e0 = { i0, i1 };
        EdgeStream[index] = e0;
    }
            
    if (GTid.x == 1)
    {
        Edge e1 = { i1, i2 };

    }
    

    
    
        GroupMemoryBarrierWithGroupSync();
    
    
    Edge e1 = { i0, i1 };
    Edge e2 = { i1, i2 };
    Edge e3 = { i0, i2 };

    EdgeStream[i] = e1;
    EdgeStream[i + 1] = e2;
    EdgeStream[i + 2] = e3;
    
    //// AABB 검출에 성공시 Divide
    
    //mDebugBuffer[DTid.xy] = float4(rayDir, 1.0f);
    
    }