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
		static NARCO_API D3DHardware* GetInstance()
		{
			static D3DHardware* instance = new D3DHardware();

			return instance;
		}
		

		NARCO_API D3DHardware(const D3DHardware& h) = delete;
		NARCO_API D3DHardware(const D3DHardware&& h) = delete;

		static NARCO_API ID3D11Device* GetDevice()
		{
			static D3DHW* inst = D3DHardware::GetInstance();
			return inst->mDevice.Get();
		}
		static NARCO_API ID3D11DeviceContext* GetImmediateContext()
		{
			static D3DHW* inst = D3DHardware::GetInstance();
			return inst->mImmediateContext.Get();
		}

	private:
		NARCO_API D3DHardware();

		ComPtr<ID3D11Device> mDevice;
		ComPtr<ID3D11DeviceContext> mImmediateContext;

		ComPtr<IDXGIFactory> mFactory;

	} D3DHW;
}

