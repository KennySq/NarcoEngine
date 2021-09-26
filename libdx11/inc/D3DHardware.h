#pragma once
#include<d3d11.h>
#include<dxgi.h>
#include<wrl.h>

#include"Common.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

namespace NARCO
{
	typedef class D3DHardware
	{
	public:
		NARCO_API D3DHardware();

		NARCO_API D3DHardware(const D3DHardware& h) = delete;
		NARCO_API D3DHardware(const D3DHardware&& h) = delete;

		NARCO_API ID3D11Device* GetDevice() const { return mDevice.Get(); }
		NARCO_API ID3D11DeviceContext* GetImmediateContext() const { return mImmediateContext.Get(); }

	private:

		ComPtr<ID3D11Device> mDevice;
		ComPtr<ID3D11DeviceContext> mImmediateContext;

		ComPtr<IDXGIFactory> mFactory;

	} D3DHW;
}

