#define MAX_DEPTH 4

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
        bool criteriaX = p.x > Min.x || p.x < Max.x;
        bool criteriaY = p.y > Min.y || p.y < Max.y;
        bool criteriaZ = p.z > Min.z || p.z < Max.z;
    
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


void ContainsEdge(AABB node, uint edgeCount, uint dtz)
{
    if (dtz < edgeCount)
    {
        Edge e = EdgeStream[dtz];
        
        //if (node.Contain(e))
        //{
        //    Divide(node);
        //}
    }
    
    GroupMemoryBarrierWithGroupSync();
}

static const float3 side0 = float3(-0.5f, 0.5f, 0.5f);
static const float3 side1 = float3(0.5f, 0.5f, 0.5f);
static const float3 side2 = float3(-0.5f, -0.5f, 0.5f);
static const float3 side3 = float3(0.5f, -0.5f, 0.5f);

static const float3 side4 = float3(-0.5f, 0.5f, -0.5f);
static const float3 side5 = float3(0.5f, 0.5f, -0.5f);
static const float3 side6 = float3(-0.5f, -0.5f, -0.5f);
static const float3 side7 = float3(0.5f, -0.5f, -0.5f);

AABB getAABB(uint3 id, AABB parentAABB)
{
    AABB tmp;
    
    //float3 min = parentAABB.Min * (side6 * depth);
    //float3 max = parentAABB.Max * (side1 * depth);
    
    float3 min = (parentAABB.Min * 0.5f); //* (id + uint3(1, 1, 1));
    float3 max = (parentAABB.Max * 0.5f); // * (id + uint3(1, 1, 1));
    
    tmp.Min = min;
    tmp.Max = max;
    
    return tmp;
}

// GTid (0 ~ 7)
// ???  (0 ~ 31)
float3 getChildIndex(uint3 GTid, uint3 Gid, uint depth)
{
    float3 inverseS = 1 / float3(32, 32, 32);
    float M = 256;
    float3 ND = pow(8, 8);
    float3 delta = depth;
    float3 p = M * ND * delta * inverseS;
    
    return p;
}

float4 Subdivide(float4 data, out AABB aabb, uint vertexCount, uint depth, uint3 GTid, uint3 Gid)
{   
        if (data.a == 0.0f)
        {
            return float4(0, 0, 0, 0);
        }
        else // when the alpha is 1.0f
        {
            for (uint i = 0; i < vertexCount; i++)
            {
                float3 p = ReadVertexBuffer[i].mPosition;

            if (aabb.Contain(p) == true)
            {   
                if (depth == (MaxDepth - 1)) // leaf depth
                {
                    float4 vertex = float4(p.x, p.y, p.z, 1.0f);
                    uint3 treeIndex = data.xyz * 31.999f * depth; // mapping to grid (32x32x32)
                     
                    OctreeTexture[treeIndex] = vertex;
                    break;
                }
                else // when it's not leaf depth
                {
                    float3 child = getChildIndex(GTid, Gid, depth + 1); // child index within the entire tree.
                    float4 newData = float4(child.x, child.y, child.z, 0.5f);
                        
                    AABB childBound = getAABB(child, aabb);
                    aabb = childBound;
                        
                    OctreeTexture[data.xyz] = newData / float4(31.999f, 31.999f, 31.999f, 1.0f); // Write child index to current node.
                    data = newData;
                    break;
                }
            }

        }
        
    // Do Specific Subdivision;
        }
    return data;
}

uint3 getChildNode(uint3 GTid, uint depth)
{
    return GTid.xyz * depth;
}

void TagNode(uint depth, uint3 GTid, uint3 DTid, uint3 Gid)
{
    float4 value = OctreeTexture[GTid.xyz];
    uint3 asIndex = value.xyz;
    
    if (value.a == 1.0f) // leaf data
    {
        if(depth < MAX_DEPTH - 1)
        {
            uint3 child = getChildNode(GTid, depth);
            float4 newValue = float4(child.xyz, 0.5f);
            
            OctreeTexture[asIndex] = newValue;
        }
        else
        {
            float4 newValue = float4(1.0, 0, 0, 1.0f);
            OctreeTexture[asIndex] = newValue;
        }
    }
    else
    {
        return;
    }

}

[numthreads(8, 8, 8)] // Eaach thread group will cover maximum voxel limit. (eg. 256^3)
void comp( uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint3 Gid : SV_GroupID, uint Gi : SV_GroupIndex)
{
    uint edgeCount;
    uint edgeStride;
    
    uint voxelCount;
    uint voxelStride;
    
    uint vertexCount;
    uint vertexStride;
    
    EdgeStream.GetDimensions(edgeCount, edgeStride);
    ReadVertexBuffer.GetDimensions(vertexCount, vertexStride);
    
    bool IsRoot = false;
    
    // clamp if this dispatch is root.
    
    int4 octreeIndex = int4(DTid.x, DTid.y, DTid.z, 0.0f); // (0, 0, 0) are fine... but how about (3, 1, 6) for root dispatch?
    float4 treeNode = OctreeTexture.Load(octreeIndex);
    
    uint3 root;
    
    if (DTid.x == 0 && DTid.y == 0 && DTid.z == 0)
    {
        root = uint3(0, 0, 0);
        OctreeTexture[root] = float4(0, 0, 0, 1.0f);
    }
    
    AllMemoryBarrierWithGroupSync();
    
    AABB aabb = { float3(-1, -1, -1), float3(1, 1, 1) };
    
    for (uint d = 0; d < MAX_DEPTH; d++)
    {
        if (DTid.x > d)
        {
            continue;
        }
        //if (DTid.x > d) // Preventing 'Out of Range' situation.
        //{
        //    continue;
        //}
       
    // check max depth
    // if max depth 
            TagNode(d, GTid, DTid, Gid);
       // treeNode = Subdivide(treeNode, aabb, vertexCount, d, GTid, Gid);
       // OctreeTexture[DTid.xyz] = 1.0f;
        //}
    }
    // if node is leaf... (where x, y, z are threated as contents)
    //      check max depth and do subdivision
    // if node is not leaf ... (which those are threated as index)
    //      keep search through the tree.
    
    AllMemoryBarrierWithGroupSync();
    
}
