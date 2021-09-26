#pragma once
#include"Common.h"
#include<d3d11.h>
#include<dxgi.h>
#include<wrl.h>


using namespace Microsoft::WRL;

struct ID3D11Device;

namespace NARCO
{
	class D3DDisplay
	{
	public:
		NARCO_API D3DDisplay(HWND windowHandle, ID3D11Device* device, unsigned int width, unsigned int height);
		NARCO_API ~D3DDisplay();
	
		NARCO_API IDXGISwapChain* GetSwapChain() const { return mSwapChain.Get(); }

		NARCO_API ID3D11RenderTargetView* GetRenderTargetView() const {
			if (mRenderTargetView == nullptr)
			{
				throw std::invalid_argument("mRenderTargetView was nullptr.");
			}
			return mRenderTargetView.Get();
		}

		NARCO_API void Present(unsigned int snycInterval = 0)
		{
			mSwapChain->Present(snycInterval, 0);
		}

		NARCO_API const D3D11_VIEWPORT& GetMainViewport() const { return *mMainViewport; }
		NARCO_API const D3D11_VIEWPORT& GetViewport(unsigned int index) const
		{
			if (index < 0 || index >= mViewports.size())
			{
				ExceptionError(E_INVALIDARG, "GetViewport() => invalid index");
				throw std::invalid_argument("Invalid index");
			}
			
			return mViewports[index];
		}



	private:
		NARCO_API D3DDisplay(const D3DDisplay& e) = delete;
		NARCO_API D3DDisplay(const D3DDisplay&& e) = delete;

		std::vector<D3D11_VIEWPORT> mViewports;
		D3D11_VIEWPORT* mMainViewport;

		ComPtr<IDXGISwapChain> mSwapChain;
		ComPtr<IDXGIFactory> mFactory;

		ComPtr<ID3D11Texture2D> mBuffer;
		ComPtr<ID3D11RenderTargetView> mRenderTargetView;

	};
}