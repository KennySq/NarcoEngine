#pragma once
#include"Common.h"

namespace NARCO
{
	struct AABB
	{
		AABB(XMFLOAT3 min, XMFLOAT3 max)
			: Min(min), Max(max)
		{
		}

		AABB(XMVECTOR min, XMVECTOR max)
		{
			XMStoreFloat3(&Min, min);
			XMStoreFloat3(&Max, max);
		}

		XMFLOAT3 Min;
		XMFLOAT3 Max;
	};

	struct Octree
	{


	public:
		Octree(ID3D11Device* device, ID3D11DeviceContext* context, const AABB& rootBound);
		
		ID3D11UnorderedAccessView* GetTextureUAV() const { return mTextureUAV.Get(); }
		ID3D11Texture3D* GetTexture() const { return mTexture.Get(); }

		void Run();

	private:
		static constexpr unsigned int ChannelSize = 0x00000020;
		
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mContext;

		AABB* mRoot;
		unsigned int mCount;
		unsigned int mDepth;
	
		std::unordered_map<size_t, AABB*> mMap;
		ComPtr<ID3D11Texture3D> mTexture;
		ComPtr<ID3D11UnorderedAccessView> mTextureUAV;


	};
}