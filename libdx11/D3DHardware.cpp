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
		DWORD flags = 0;
#ifdef _DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;

#endif

		HRESULT result;

		result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags,
			featureLevels, featureLevelCount, D3D11_SDK_VERSION,
			mDevice.GetAddressOf(), &succeededLevel, mImmediateContext.GetAddressOf());
	
		if (result != S_OK)
		{
			Debug::Throw("result was not S_OK");
		}
	}
}
