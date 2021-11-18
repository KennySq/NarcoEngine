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
    uint A;
    uint B;
};

StructuredBuffer<Vertex_Input> ReadVertexBuffer : register(t0);
StructuredBuffer<Edge> EdgeStream : register(t1);

bool CompareEdge(Edge e1, Edge e2)
{
    return (e1.A == e2.A && e1.B == e2.B);
}

struct AABB
{
    float3 Min;
    float3 Max;
    
    bool Contain(Edge e)
    {
        float3 v1 = ReadVertexBuffer[e.A].mPosition;
        float3 v2 = ReadVertexBuffer[e.B].mPosition;
        
        bool criteriaX = v2.x < Min.x || v1.x > Max.x;
        bool criteriaY = v2.y < Min.y || v1.y > Max.y;
        bool criteriaZ = v2.z < Min.z || v1.z > Max.z;
        
        return (criteriaX || criteriaY || criteriaZ);
    }
       
};

Texture3D<uint> OctreeTexture : register(u2);   // Tree
RWStructuredBuffer<AABB> VoxelStream : register(u3); // Single Voxel Slice

void Divide(AABB node)
{
    AABB n0, n1, n2, n3, n4, n5, n6, n7;
    
    n0.Min = node.Min;
    n0.Max = node.Max * 0.5f;
    
    n1.Min = float3(node.Min.x + n0.Max.x, n0.Min.y, n0.Min.z);
    n1.Max = float3(node.Max.x, n0.Max.y, n0.Max.z);
    
    n2.Min = float3(node.Min.x, n0.Max.y, n0.Min.z);
    n2.Max = float3(node.Min.x, node.Max.y, n0.Max.z);
    
    n3.Min = float3(n0.Max.x, n0.Max.y, n0.Min.z);
    n3.Max = float3(node.Max.x, node.Max.y, n0.Max.z);
    
    n4.Min = float3(n0.Min.x, n0.Min.y, n0.Max.z);
    n4.Max = float3(n0.Max.x, n0.Max.y, node.Max.z);
    
    n5.Min = float3(n1.Min.x, n1.Min.y, n1.Max.z);
    n5.Max = float3(n1.Max.x, n1.Max.y, node.Max.z);
    
    n6.Min = float3(n2.Min.x, n2.Min.y, n2.Max.z);
    n6.Max = float3(n2.Max.x, n2.Max.y, node.Max.z);
    
    n7.Min = float3(n3.Min.x, n3.Min.y, n3.Max.z);
    n7.Max = float3(n3.Max.x, n3.Max.y, node.Max.z);
    
    
       
}

void ContainsEdge(AABB node, uint edgeCount, uint dtz)
{
    if (dtz < edgeCount)
    {
        Edge e = EdgeStream[dtz];
        
        if (node.Contain(e))
        {
            Divide(node);
        }
    }
    
    GroupMemoryBarrierWithGroupSync();
}

[numthreads(32, 32, 32)]
void comp( uint3 DTid : SV_DispatchThreadID, uint3 Gid : SV_GroupID, uint Gi : SV_GroupIndex)
{
    uint edgeCount;
    uint edgeStride;
    
    uint voxelCount;
    uint voxelStride;
    
    EdgeStream.GetDimensions(edgeCount, edgeStride);
    VoxelStream.GetDimensions(voxelCount, voxelStride);
    
    if (Gi < voxelCount) // Dispatch count가 voxel slice 보다 작아야함
    {
        AABB node = VoxelStream[DTid.x];
        ContainsEdge(node, edgeCount, DTid.z);      

    }
    
    
    AllMemoryBarrierWithGroupSync();
    
}
