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

		bool operator==(const Vertex_Static& v) const
		{
			bool bPosition = (mPosition.x == v.mPosition.x) && (mPosition.y == v.mPosition.y) && (mPosition.z == v.mPosition.z);
			bool bNormal = (mNormal.x == v.mNormal.x) && (mNormal.y == v.mNormal.y) && (mNormal.z == v.mNormal.z);
			bool bBinormal = (mBinormal.x == v.mBinormal.x) && (mBinormal.y && v.mBinormal.y) && (mBinormal.z == v.mBinormal.z);
			bool bTangent = (mTangent.x == v.mTangent.x) && (mTangent.y && v.mTangent.y) && (mTangent.z == v.mTangent.z);
			bool bTexcoord = (mTexcoord.x == v.mTexcoord.x) && (mTexcoord.y && v.mTexcoord.y);

			return bPosition && bNormal && bBinormal && bTangent && bTexcoord;
		}
	};

	enum eVertexType
	{
		VERTEX_STATIC,
		VERTEX_SKELETAL,
	};

	class Mesh
	{
		friend MeshLoader;

	public:
		Mesh();
		Mesh(const Mesh& mesh) = delete;
		Mesh(const Mesh&&) = delete;

		ID3D11Buffer* GetVertex() const { return mVertex.Get(); }
		ID3D11Buffer* GetIndex() const { return mIndex.Get(); }

	private:
		ComPtr<ID3D11Buffer> mVertex;
		ComPtr<ID3D11Buffer> mIndex;
	};
}