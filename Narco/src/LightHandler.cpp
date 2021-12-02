#include"../inc/LightHandler.h"

namespace NARCO
{
	LightHandler* LightHandler::mInstance = nullptr;
	LightHandler::LightHandler()
	{
		HRESULT result = mDevice->CreateClassLinkage(mLinkage.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create class linkage");
			return;
		}

		D3D11_BUFFER_DESC directionalDesc{};

		directionalDesc.ByteWidth = sizeof(DirectionalLight::Raw) * 1024;
		// update from here, 2021/12/02 5:44 PM


	}
	LightHandler::~LightHandler()
	{
	}
};