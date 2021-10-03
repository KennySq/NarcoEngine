#pragma once
#include"Common.h"

namespace NARCO
{
	class MeshLoader;
	struct Vertex_Static
	{
		XMFLOAT3 mPosition;
		XMFLOAT3 mNormal;
		XMFLOAT3 mBinormal;
		XMFLOAT3 mTangent;
		XMFLOAT2 mTexcoord;

		friend std::hash<Vertex_Static>;
		bool operator==(const Vertex_Static& v2) const
		{
			bool bPosition =	(mPosition.x == v2.mPosition.x) && (mPosition.y ==	v2.mPosition.y) && (mPosition.z ==	v2.mPosition.z);
			bool bNormal =		(mNormal.x ==	v2.mNormal.x)	&& (mNormal.y ==	v2.mNormal.y) && (mNormal.z ==		v2.mNormal.z);
			//bool bBinormal =	(mBinormal.x == v2.mBinormal.x) && (mBinormal.y ==	v2.mBinormal.y) && (mBinormal.z ==	v2.mBinormal.z);
			//bool bTangent =			(mTangent.x ==	v2.mTangent.x)	&& (mTangent.y ==	v2.mTangent.y) && (mTangent.z ==	v2.mTangent.z);
			//bool bTexcoord =	(mTexcoord.x == v2.mTexcoord.x) && (mTexcoord.y ==	v2.mTexcoord.y);


			return bPosition && bNormal; //&& bBinormal && bTangent && bTexcoord;

		}
	};



	struct Vertex_Quad
	{
		XMFLOAT3 mPosition;
		XMFLOAT2 mTexcoord;
	};

	enum eVertexType
	{
		VERTEX_STATIC,
		VERTEX_SKELETAL,
	};

	enum eMeshPrimitive
	{
		PRIMITIVE_QUAD,
		PRIMITIVE_SPHERE,
		PRIMITIVE_QUBE,
		//...
	};

	class Mesh
	{
		friend MeshLoader;

	public:
		Mesh();
		Mesh(ID3D11Device* device, eMeshPrimitive primitiveTag);
		Mesh(const Mesh& mesh) = delete;
		Mesh(const Mesh&&) = delete;

		ID3D11Buffer* GetVertex() const { return mVertex.Get(); }
		ID3D11Buffer* GetIndex() const { return mIndex.Get(); }

		unsigned int GetStride() const { return mStride; }

		unsigned int GetIndexCount() const { return mIndexCount; }

	private:
		ComPtr<ID3D11Buffer> mVertex;
		ComPtr<ID3D11Buffer> mIndex;

		unsigned int mStride;
		unsigned int mIndexCount;
	};
}

namespace std
{
	template<>
	class std::hash<NARCO::Vertex_Static>
	{
	public:
		size_t operator()(const NARCO::Vertex_Static& v) const
		{
			using std::hash;
			
			hash<float> h;

			

			size_t posx = h(v.mPosition.x);
			size_t posy = h(v.mPosition.y);
			size_t posz = h(v.mPosition.z);

			size_t normx = h(v.mNormal.x);
			size_t normy = h(v.mNormal.y);
			size_t normz = h(v.mNormal.z);

			size_t binormx = h(v.mBinormal.x);
			size_t binormy = h(v.mBinormal.y);
			size_t binormz = h(v.mBinormal.z);
			
			size_t tanx = h(v.mTangent.x);
			size_t tany = h(v.mTangent.y);
			size_t tanz = h(v.mTangent.z);
			
			size_t uvx = h(v.mTexcoord.x);
			size_t uvy = h(v.mTexcoord.y);

			return (posx ^ posy ^ posz ^ normx ^ normy ^ normz ^ binormx ^ binormy ^ binormz ^ tanx ^ tany ^ tanz ^ uvx ^ uvy);
			//return (uvy ^ uvx ^ tanz ^ tany ^ tanx ^ binormz ^ binormy ^ binormx ^ normz ^ normy ^ normx ^ posz ^ posy ^ posx) << 1;
			//return (posx ^ posy ^ posz);
		}
	};
}

