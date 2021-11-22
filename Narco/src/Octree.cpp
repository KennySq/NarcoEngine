#include"../inc/Octree.h"

namespace NARCO
{
	Octree::Octree(ID3D11Device* device, ID3D11DeviceContext* context, const AABB& rootBound)
		: mDevice(device), mContext(context), mRoot(new AABB(rootBound))
	{
		D3D11_TEXTURE3D_DESC octreeTexDesc{};
		D3D11_UNORDERED_ACCESS_VIEW_DESC octreeTexUAVDesc{};
		
		octreeTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		octreeTexDesc.Width = ChannelSize; // 2^6 
		octreeTexDesc.Height = ChannelSize;
		octreeTexDesc.Depth = ChannelSize;

		octreeTexDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		octreeTexDesc.MipLevels = 1;
		
		octreeTexUAVDesc.Format = octreeTexDesc.Format;
		octreeTexUAVDesc.Texture3D.WSize = ChannelSize;
		octreeTexUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
		HRESULT result = mDevice->CreateTexture3D(&octreeTexDesc, nullptr, mTexture.GetAddressOf());

		if (result != S_OK)
		{
			Debug::Log("failed to create texture 3d");
			return;
		}

		result = mDevice->CreateUnorderedAccessView(mTexture.Get(), &octreeTexUAVDesc, mTextureUAV.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create texture 3d unordered access view.");
			return;
		}
	}
}