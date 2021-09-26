#include"D3DDepthStencil.h"

namespace NARCO
{
	D3DDepthStencil::D3DDepthStencil(ID3D11Device* device, D3D11_BIND_FLAG bindFlags, D3D11_USAGE usage, unsigned int width, unsigned int height, unsigned int mipLevels, unsigned int cpuAccessFlag)
		: mWidth(width), mHeight(height), mHostAccess((D3D11_CPU_ACCESS_FLAG)cpuAccessFlag)
	{
		D3D11_TEXTURE2D_DESC desc{};
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

		desc.Usage = usage;
		desc.BindFlags = (bindFlags | D3D11_BIND_DEPTH_STENCIL);
		desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		desc.MipLevels = mipLevels;
		desc.SampleDesc.Count = 1;
		desc.ArraySize = 1;
		desc.Width = width;
		desc.Height = height;


		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		HRESULT result = device->CreateTexture2D(&desc, nullptr, mTexture.GetAddressOf());
		ExceptionError(result, "Creating ID3D11Texture2D");

		if (result != S_OK)
		{
			return;
		}

		result = device->CreateDepthStencilView(mTexture.Get(), &dsvDesc, mDepthStencilView.GetAddressOf());
		ExceptionError(result, "Creating ID3D11DepthStencilView.");

		if (result != S_OK)
		{
			mTexture->Release();

			return;
		}

		if ((bindFlags & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE) == 1)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

			srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

			result = device->CreateShaderResourceView(mTexture.Get(), &srvDesc, mShaderResourceView.GetAddressOf());
			ExceptionError(result, "Creating Shader Resource View.");

			if (result != S_OK)
			{
				if (mDepthStencilView != nullptr)
				{
					mDepthStencilView->Release();
				}

				mTexture->Release();
				
				return;
			}

		}


	}
	D3DDepthStencil::~D3DDepthStencil()
	{
		if (mTexture != nullptr)
		{
			mTexture->Release();
		}

		if (mDepthStencilView != nullptr)
		{
			mDepthStencilView->Release();
		}

		if (mShaderResourceView != nullptr)
		{
			mShaderResourceView->Release();
		}
	}
}