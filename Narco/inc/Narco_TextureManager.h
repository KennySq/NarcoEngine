#pragma once
#include"D3DTexture2D.h"
#include"D3DDepthStencil.h"

#include<vector>

using namespace NARCO;

namespace NARCO
{
	// ### WARNING ! ### Narco_TextureManager is no longer used, Use GBuffer instead.
	class __declspec(deprecated) Narco_TextureManager
	{
	public:
		__declspec(deprecated) void AddTexture2D(D3DTexture2D* p);
		__declspec(deprecated) void AddDepthStencil(D3DDepthStencil* p);

		__declspec(deprecated) D3DTexture2D* GetTexture2D(unsigned int index);
		__declspec(deprecated) D3DDepthStencil* GetDepthStencil(unsigned int index);

		__declspec(deprecated) ID3D11RenderTargetView** GetRenderTargets()
		{
			return mRenderTargets.data();
		}

	private:
		std::vector<D3DTexture2D*> mListTexture2D;
		std::vector<D3DDepthStencil*> mListDepthStencils;

		std::vector<ID3D11RenderTargetView*> mRenderTargets;
		std::vector<ID3D11ShaderResourceView*> mShaderResources;

	};
}