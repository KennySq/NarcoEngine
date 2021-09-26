#pragma once
#include"Common.h"
#include"D3DHardware.h"

namespace NARCO
{
	class D3DDepthStencil
	{
	public:
		NARCO_API D3DDepthStencil(ID3D11Device* device, D3D11_BIND_FLAG bindFlags, D3D11_USAGE usage, unsigned int width, unsigned int height, unsigned int mipLevels, unsigned int cpuAccessFlag = 0);
		NARCO_API ~D3DDepthStencil();

		NARCO_API ID3D11DepthStencilView* GetDepthStencilView() const { return mDepthStencilView.Get(); }

	private:
		unsigned int mWidth;
		unsigned int mHeight;
		
		D3D11_CPU_ACCESS_FLAG mHostAccess;


		ComPtr<ID3D11Texture2D> mTexture = nullptr;

		ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;
		ComPtr<ID3D11ShaderResourceView> mShaderResourceView = nullptr;
	};
}