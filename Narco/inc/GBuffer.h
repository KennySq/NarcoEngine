#pragma once
#include"Common.h"

#include"D3DTexture2D.h"
#include"D3DDepthStencil.h"

namespace NARCO
{
	static DXGI_FORMAT GBufferFormats[] =
	{
		DXGI_FORMAT_R32G32B32A32_FLOAT, // Position
		DXGI_FORMAT_R32G32B32A32_FLOAT, // Normal
		DXGI_FORMAT_R32G32_FLOAT,		// UV
	};

	class GBuffer
	{
	public:
		GBuffer(ID3D11Device* device, unsigned int width, unsigned int height);
		~GBuffer();

		ID3D11RenderTargetView* const * GetRenderTargets() { return mRenderTargets.data(); }
		ID3D11DepthStencilView* GetDepthStencil() { return mDepth->GetDepthStencilView(); }

		const unsigned int GetBufferCount() const { return mBufferCount; }

	private:
		ID3D11Device* mDevice;

		std::vector<D3DTexture2D*> mBuffers;
		D3DDepthStencil* mDepth;

		std::vector<ID3D11RenderTargetView*> mRenderTargets;
		std::vector<ID3D11ShaderResourceView*> mShaderResources;

		const unsigned int mBufferCount;
		
		unsigned int mWidth;
		unsigned int mHeight;
	};
}