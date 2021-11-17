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

struct Edge 
{
    float3 A; // From
    float3 B; // To
};

bool CompareEdge

struct Triangle
{
    uint i0, i1, i2;
};

//Triangle GetAdjecentTriangle(Triangle tri)
//{
    
//}

struct AABB
{
    float3 Min;
    float3 Max;
    
    bool Contain(Edge e)
    {
        bool criteriaX = e.B.x < Min.x || e.A.x > Max.x;
        bool criteriaY = e.B.y < Min.y || e.A.y > Max.y;
        bool criteriaZ = e.B.z < Min.z || e.A.z > Max.z;
        
        return (criteriaX || criteriaY || criteriaZ);
    }
};

struct Ray
{
    float3 Origin;
    float3 Direction;
    
    float3 At(float t)
    {
        return Origin + Direction * t;
    }
};

cbuffer Constants : register(b0)
{
    float4x4 View;
    float4x4 Projection;
    
    float4 ViewPosition;
    float4 ViewDirection;
}

StructuredBuffer<Vertex_Input> ReadVertexBuffer : register(t0);
Buffer<uint> ReadTriangleBuffer : register(t1);
StructuredBuffer<AABB> InputOctree : register(t2);

RWTexture2D<float4> mDebugBuffer : register(u0);
RWStructuredBuffer<AABB> OctreeStream : register(u2);
RWStructuredBuffer<Edge> EdgeStream : register(u3);


[numthreads(32, 30, 1)]
void comp( uint3 DTid : SV_DispatchThreadID, uint3 Gid : SV_GroupThreadID, uint Gi : SV_GroupIndex)
{
    // 1280, 720
    float2 resolution = float2(1280, 720); // (resolution / DTid.xy)
    float2 uv = (DTid.xy + uint2(1, 1)) / resolution;
 /*
    		eye = XMVectorSet(-1, 0, 0, 0);
		up = XMVectorSet(0, 1, 0, 0);
		at = XMVectorSet(0, 0, 0, 0);
    */
    float3 rayOrigin = float3(ViewPosition.xyz);
    float3 rayDir = float3(ViewDirection.xyz) + float3(uv, 0.0f);
    
    Ray r = { rayOrigin, rayDir };
    
    
    // edge 테이블 초기화
    uint indexCount;
    uint triangleCount;
    ReadTriangleBuffer.GetDimensions(indexCount);
    
    triangleCount = indexCount / 3;
    
    for (uint i = 0; i < triangleCount; i++)
    {
        uint i0 = ReadTriangleBuffer[i];
        uint i1 = ReadTriangleBuffer[i + 1];
        uint i2 = ReadTriangleBuffer[i + 2];
        
        Edge e1 = { i0, i1 };
        Edge e2 = { i1, i2 };
        Edge e3 = { i0, i2 };

        EdgeStream[]
        
    }
    
    
    // AABB 검출에 성공시 Divide
    
        mDebugBuffer[DTid.xy] = float4(rayDir, 1.0f);
    
    }