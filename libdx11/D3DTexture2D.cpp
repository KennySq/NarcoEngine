#include "inc\D3DTexture2D.h"


namespace NARCO
{
	D3DTexture2D::D3DTexture2D(ID3D11Device* device, DXGI_FORMAT format, unsigned int bindFlag, D3D11_USAGE usage, unsigned int width, unsigned int height, unsigned int mipLevels, unsigned int cpuAccessFlag)
		: mWidth(width), mHeight(height), mHostAccess((D3D11_CPU_ACCESS_FLAG)cpuAccessFlag), mBindFlags(bindFlag)
	{
		D3D11_TEXTURE2D_DESC desc{};

		desc.Format = format;
		desc.MipLevels = mipLevels;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = usage;
		desc.BindFlags = bindFlag;
		desc.CPUAccessFlags = cpuAccessFlag;
		desc.Width = width;
		desc.Height = height;
		
		HRESULT result = device->CreateTexture2D(&desc, nullptr, mTexture.GetAddressOf());

		if (result != S_OK)
		{
			Debug::Log("result was not S_OK");

			return;
		}

		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
		{
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};

			rtvDesc.Format = format;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			
			result = device->CreateRenderTargetView(mTexture.Get(), &rtvDesc, mRenderTargetView.GetAddressOf());
			if (result != S_OK)
			{
				Debug::Log("result was not S_OK");

				mTexture->Release();

				return;

			}
		}


		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

			srvDesc.Format = format;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = mipLevels;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

			result = device->CreateShaderResourceView(mTexture.Get(), &srvDesc, mShaderResourceView.GetAddressOf());
			if (result != S_OK)
			{
				Debug::Log("result was not S_OK");


				if (mRenderTargetView != nullptr)
				{
					mRenderTargetView->Release();
				}

				mTexture->Release();

				return;
			}
		}

		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

			uavDesc.Format = format;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

			result = device->CreateUnorderedAccessView(mTexture.Get(), &uavDesc, mUnorderedAccessView.GetAddressOf());
			
			if (result != S_OK)
			{
				Debug::Log("result was not S_OK");

				if (mRenderTargetView != nullptr)
				{
					mRenderTargetView->Release();
				}
				if (mShaderResourceView != nullptr)
				{
					mShaderResourceView->Release();
				}

				mTexture->Release();

				return;
			}

		}
	}

	D3DTexture2D::~D3DTexture2D()
	{
		if (mTexture != nullptr)
		{
			mTexture->Release();
		}

		if (mShaderResourceView != nullptr)
		{
			mShaderResourceView->Release();
		}

		if (mRenderTargetView != nullptr)
		{
			mRenderTargetView->Release();
		}

		if (mUnorderedAccessView != nullptr)
		{
			mUnorderedAccessView->Release();
		}
	}


	D3DTexture2DArray::D3DTexture2DArray(ID3D11Device* device, DXGI_FORMAT format, unsigned int arraySize, unsigned int bindFlag, D3D11_USAGE usage, unsigned int width, unsigned int height, unsigned int mipLevels, unsigned int cpuAccessFlag)
	{
		D3D11_TEXTURE2D_DESC texDesc{};
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

		texDesc.ArraySize = arraySize;
		texDesc.BindFlags = bindFlag;
		texDesc.CPUAccessFlags = cpuAccessFlag;
		texDesc.Format = format;
		texDesc.MipLevels = 1;
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.SampleDesc.Count = 1;

		rtvDesc.Format = texDesc.Format;
		rtvDesc.Texture2DArray.ArraySize = arraySize;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;

		srvDesc.Format = texDesc.Format;
		srvDesc.Texture2DArray.MipLevels = 1;
		srvDesc.Texture2DArray.ArraySize = arraySize;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;

		HRESULT result = device->CreateTexture2D(&texDesc, nullptr, mTexture.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create texture 2d array.");
			return;
		}

		if (bindFlag & D3D11_BIND_SHADER_RESOURCE)
		{
			result = device->CreateShaderResourceView(mTexture.Get(), &srvDesc, mShaderResourceView.GetAddressOf());
			if (result != S_OK)
			{
				mTexture.ReleaseAndGetAddressOf();

				Debug::Log("failed to create shader resource view.");
				return;
			}
		}

		if (bindFlag & D3D11_BIND_RENDER_TARGET)
		{
			result = device->CreateRenderTargetView(mTexture.Get(), &rtvDesc, mRenderTargetView.GetAddressOf());
			if (result != S_OK)
			{
				mTexture.ReleaseAndGetAddressOf();

				if (mShaderResourceView != nullptr)
				{
					mShaderResourceView.ReleaseAndGetAddressOf();
				}

				Debug::Log("failed to create render target view.");
				return;
			}
		}
	}

	D3DTexture2DArray::~D3DTexture2DArray()
	{
		if (mTexture != nullptr)
		{
			mTexture->Release();
		}

		if (mShaderResourceView != nullptr)
		{
			mShaderResourceView->Release();
		}

		if (mRenderTargetView != nullptr)
		{
			mRenderTargetView->Release();
		}

		if (mUnorderedAccessView != nullptr)
		{
			mUnorderedAccessView->Release();
		}
	}
}