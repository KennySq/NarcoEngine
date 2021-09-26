#include "inc/D3DHardware.h"

namespace NARCO
{
	D3DHardware::D3DHardware()
	{
		const D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		const unsigned int featureLevelCount = ARRAYSIZE(featureLevels);

		D3D_FEATURE_LEVEL succeededLevel;

		HRESULT result;

		result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
			featureLevels, featureLevelCount, D3D11_SDK_VERSION,
			mDevice.GetAddressOf(), &succeededLevel, mImmediateContext.GetAddressOf());
	
		ExceptionError(result, "creating ID3D11Device, ID3D11DeviceContext");
	}
}
