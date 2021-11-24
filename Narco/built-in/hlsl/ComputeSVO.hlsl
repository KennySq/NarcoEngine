#define MAX_DEPTH 8

cbuffer Constants : register(b0)
{
    float4x4 v;
    float4x4 p;
    unsigned int CurrentDepth;
    unsigned int MaxDepth;
}

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

bool CompareEdge(Edge e1, Edge e2)
{
    return (e1.A == e2.A && e1.B == e2.B);
}



StructuredBuffer<Vertex_Input> ReadVertexBuffer : register(t0);
//Buffer<uint> ReadTriangleBuffer : register(t1);

struct AABB
{
    float3 Min;
    float3 Max;
    
    //bool Contain(Edge e)
    //{
    //    float3 v1 = ReadVertexBuffer[e.A].mPosition;
    //    float3 v2 = ReadVertexBuffer[e.B].mPosition;
        
    //    bool criteriaX = v2.x < Min.x || v1.x > Max.x;
    //    bool criteriaY = v2.y < Min.y || v1.y > Max.y;
    //    bool criteriaZ = v2.z < Min.z || v1.z > Max.z;
        
    //    return (criteriaX || criteriaY || criteriaZ);
    //}
    
    bool Contain(float3 p)
    {
        bool criteriaX = p.x >= Min.x && p.x <= Max.x;
        bool criteriaY = p.y >= Min.y && p.y <= Max.y;
        bool criteriaZ = p.z >= Min.z && p.z <= Max.z;
    
        return (criteriaX && criteriaY && criteriaZ);
    }
};

StructuredBuffer<Edge> EdgeStream : register(t1);
//StructuredBuffer<AABB> VoxelBuffer : register(t2); // Total Voxel Octree Buffer
RWTexture2D<float4> DebugTextrue : register(u0);
RWTexture3D<unorm float4> OctreeTexture : register(u1);   // Tree Indexing Texture, it's uint4 but only 8 bits for each component.
//AppendStructuredBuffer<AABB> VoxelSlice : register(u3); // Single Voxel Slice for current division depth

//void Divide(AABB node)
//{
//    AABB n0, n1, n2, n3, n4, n5, n6, n7;
    
//    n0.Min = node.Min;
//    n0.Max = node.Max * 0.5f;
    
//    n1.Min = float3(node.Min.x + n0.Max.x, n0.Min.y, n0.Min.z);
//    n1.Max = float3(node.Max.x, n0.Max.y, n0.Max.z);
    
//    n2.Min = float3(node.Min.x, n0.Max.y, n0.Min.z);
//    n2.Max = float3(node.Min.x, node.Max.y, n0.Max.z);
    
//    n3.Min = float3(n0.Max.x, n0.Max.y, n0.Min.z);
//    n3.Max = float3(node.Max.x, node.Max.y, n0.Max.z);
    
//    n4.Min = float3(n0.Min.x, n0.Min.y, n0.Max.z);
//    n4.Max = float3(n0.Max.x, n0.Max.y, node.Max.z);
    
//    n5.Min = float3(n1.Min.x, n1.Min.y, n1.Max.z);
//    n5.Max = float3(n1.Max.x, n1.Max.y, node.Max.z);
    
//    n6.Min = float3(n2.Min.x, n2.Min.y, n2.Max.z);
//    n6.Max = float3(n2.Max.x, n2.Max.y, node.Max.z);
    
//    n7.Min = float3(n3.Min.x, n3.Min.y, n3.Max.z);
//    n7.Max = float3(n3.Max.x, n3.Max.y, node.Max.z);

//}

//AABB getAABB(uint3 id, AABB parentAABB)
//{
//    AABB tmp;
    
//    //float3 min = parentAABB.Min * (side6 * depth);
//    //float3 max = parentAABB.Max * (side1 * depth);
    
//    float3 min = (parentAABB.Min * 0.5f); //* (id + uint3(1, 1, 1));
//    float3 max = (parentAABB.Max * 0.5f); // * (id + uint3(1, 1, 1));
    
//    tmp.Min = min;
//    tmp.Max = max;
    
//    return tmp;
//}

//// GTid (0 ~ 7)
//// ???  (0 ~ 31)
//float3 getChildIndex(uint3 GTid, uint3 Gid, uint depth)
//{
//    float3 inverseS = 1 / float3(32, 32, 32);
//    float M = 256;
//    float3 ND = pow(8, 8);
//    float3 delta = depth;
//    float3 p = M * ND * delta * inverseS;
    
//    return p;
//}

//uint3 getChildNode(uint3 DTid, uint depth, float3 vertex)
//{
//    float n = pow(8, depth);
//    float3 id = DTid.xyz + frac(vertex * n); // 
//    return id / 32;
//}

float3 getChildNode(uint3 index, uint depth, float3 vertex)
{
    float3 m = vertex;
    float n = pow(8, depth);
    float3 id = index + frac(vertex * n);
    return id / (32 * 32 * 32);
    //float s = 1.0f /(32.0f * 32.0f * 32.0f);
    //float3 p = m * n *   * s;
    
}

[numthreads(8,8,8)]
void comp(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint3 Gid : SV_GroupID, uint Gi : SV_GroupIndex)
{
    OctreeTexture[DTid] = float4(0, 0, 0, 1);
    
    uint vertexCount, vertexStride;
    uint3 globalIndex = DTid;
    uint3 localIndex = GTid;
    uint3 localGridIndex = Gid;
    
    ReadVertexBuffer.GetDimensions(vertexCount, vertexStride);
    
    for (uint i = 0; i < MAX_DEPTH; i++)
    {
        //if (DTid.x > i || DTid.y > i || DTid.z > i)
        //{
        //    continue;
        //}
        
        float4 value = OctreeTexture[globalIndex];
        
        if (value.a == 1.0f)
        {
            for (uint v = 0; v < vertexCount; v++)
            {
                float3 vertex = ReadVertexBuffer[v].mPosition;
                if (i < MAX_DEPTH) // not leaf
                {
                    float3 child = getChildNode(localGridIndex, i, vertex);
                    OctreeTexture[globalIndex] = float4(floor(child.xyz), 0.5f);
                    localIndex = child;
                
                 //   break;
              //  OctreeTexture[index] = float4(1, 1, 1, 1);

                }
                else // leaf
                {
                    OctreeTexture[localIndex] = float4(vertex, 1.0f);
               // OctreeTexture[index] = float4(1, 1, 0, 1.0f);

                }
            }
        }
        
    }
    
    
    AllMemoryBarrierWithGroupSync
    ();

}