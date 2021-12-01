#include"../inc/LightHandler.h"

namespace NARCO
{
	LightHandler::LightHandler()
	{
		HRESULT result = mDevice->CreateClassLinkage(mLinkage.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create class linkage");
			return;
		}


	}
	LightHandler::~LightHandler()
	{
	}
	XMFLOAT3 PointLight::Diffuse(XMFLOAT3 normal)
	{
		
		return XMFLOAT3();
	}
	XMFLOAT3 DirectionalLight::Diffuse(XMFLOAT3 normal)
	{
		// dot product?
		return XMFLOAT3();
	}
	void DirectionalLight::update(float delta)
	{
		Light::update(delta);
	
		XMStoreFloat3(&mDirection, mTransform->GetRotation());
	}
};