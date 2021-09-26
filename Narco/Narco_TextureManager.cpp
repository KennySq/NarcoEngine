#include"inc/Narco_TextureManager.h"

namespace NARCO
{
	__declspec(deprecated) void Narco_TextureManager::AddTexture2D(D3DTexture2D* p)
	{
		if (p == nullptr)
		{
			ExceptionError(E_INVALIDARG, "invalid address");
			return;
		}

		unsigned int bindFlag = p->GetBindFlags();

		if (bindFlag & D3D11_BIND_RENDER_TARGET)
		{
			auto rtv = p->GetRenderTarget();

			mRenderTargets.emplace_back(rtv);
		}

		mListTexture2D.emplace_back(p);
	}
	__declspec(deprecated) void Narco_TextureManager::AddDepthStencil(D3DDepthStencil* p)
	{
		if (p == nullptr)
		{
			ExceptionError(E_INVALIDARG, "invalid address");
			return;
		}

		mListDepthStencils.emplace_back(p);

	}

	__declspec(deprecated) D3DTexture2D* Narco_TextureManager::GetTexture2D(unsigned int index)
	{
		if (index >= mListTexture2D.size() || index < 0)
		{
			ExceptionError(E_INVALIDARG, "invalid index");
			return nullptr;
		}
		return mListTexture2D[index];
	}
	__declspec(deprecated) D3DDepthStencil* Narco_TextureManager::GetDepthStencil(unsigned int index)
	{
		if (index >= mListDepthStencils.size() || index < 0)
		{
			ExceptionError(E_INVALIDARG, "invalid index");
			return nullptr;
		}

		return mListDepthStencils[index];
	}
}