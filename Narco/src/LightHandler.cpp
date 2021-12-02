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


	}
	LightHandler::~LightHandler()
	{
	}
	bool LightHandler::GenerateDirectionals(std::vector<DirectionalLight>& lights)
	{
		D3D11_BUFFER_DESC directionalBufferDesc{};

		directionalBufferDesc.ByteWidth = sizeof(DirectionalLight) * MAX_DIRECTIONAL_LIGHT;
		directionalBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		directionalBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		directionalBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA subData{};

		subData.pSysMem = lights.data();

		HRESULT result = mDevice->CreateBuffer(&directionalBufferDesc, &subData, mDirectionalLightBuffer.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to generate real-time light buffer.");
			return false;
		}

		return true;
	}
	bool LightHandler::GeneratePoints(std::vector<PointLight>& lights)
	{
		D3D11_BUFFER_DESC pointBufferDesc{};

		pointBufferDesc.ByteWidth = sizeof(PointLight) * MAX_POINT_LIGHT;
		pointBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		pointBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		pointBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA subData{};

		subData.pSysMem = lights.data();

		HRESULT result = mDevice->CreateBuffer(&pointBufferDesc, &subData, mPointLightBuffer.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to generate real-time light buffer.");
			return false;
		}


	}

	//XMFLOAT3 PointLight::Diffuse(XMFLOAT3 normal)
	//{
	//	
	//	return XMFLOAT3();
	//}
	//XMFLOAT3 DirectionalLight::Diffuse(XMFLOAT3 normal)
	//{
	//	float ndotl = dot(normal, XMFLOAT3(1, 1, 1));
	//	
	//	if (ndotl > 0.0f)
	//	{
	//		return XMFLOAT3(1, 0, 0);
	//	}
	//	else
	//	{
	//		return XMFLOAT3(1, 1, 0);
	//	}
	//	
	//	return XMFLOAT3(1, 0, 1);
	//}
	//void DirectionalLight::update(float delta)
	//{
	//	Light::update(delta);
	//
	//	XMStoreFloat3(&mDirection, mTransform->GetRotation());
	//}
};