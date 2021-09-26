#include "D3DDisplay.h"

namespace NARCO
{
	D3DDisplay::D3DDisplay(HWND windowHandle, ID3D11Device* device, unsigned int width, unsigned int height)
	{
		HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(mFactory.GetAddressOf()));
		ExceptionError(result, "creating IDXGIFactory");
		
		if (result != S_OK)
		{
			return;
		}

		DXGI_SWAP_CHAIN_DESC swapChainDesc{};

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 144;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.OutputWindow = windowHandle;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		result = mFactory->CreateSwapChain(reinterpret_cast<IUnknown*>(device), &swapChainDesc, mSwapChain.GetAddressOf());
		ExceptionError(result, "Creating IDXGISwapChain");
		
		if (result != S_OK)
		{
			mFactory->Release();
			return;
		}

		result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(mBuffer.GetAddressOf()));
		ExceptionError(result, "Getting buffer from swapchain");

		if (result != S_OK)
		{
			mFactory->Release();
			mSwapChain->Release();
			
			return;
		}

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};

		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		result = device->CreateRenderTargetView(mBuffer.Get(), &rtvDesc, mRenderTargetView.GetAddressOf());
		ExceptionError(result, "Creating render target view.");

		if (result != S_OK)
		{
			mFactory->Release();
			mSwapChain->Release();
			mBuffer->Release();
			
			return;
		}

		D3D11_VIEWPORT viewport;

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		viewport.Width = width;
		viewport.Height = height;

		mViewports.emplace_back(viewport);

		mMainViewport = &mViewports[0];
	}

	D3DDisplay::~D3DDisplay()
	{
		mFactory->Release();
		mSwapChain->Release();

	}
}