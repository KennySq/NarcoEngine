
#include "inc/Light.h"

namespace NARCO
{
	Light::Light(eLightType type, eLightMode mode)
		: mMode(mode), mType(type)
	{
		HRESULT result;
		D3D11_BUFFER_DESC bufferDesc{};
		D3D11_SUBRESOURCE_DATA subData{};

		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		Directional directional{ {1,0,0}, 1 };
		Point point{ { 1,0,0}, 1 };

		if (type == LIGHT_POINT)
		{
			bufferDesc.ByteWidth = sizeof(Point);
			subData.pSysMem = &point;
		}

		else if (type == LIGHT_DIRECTIONAL)
		{
			bufferDesc.ByteWidth = sizeof(Directional);
			subData.pSysMem = &directional;
		}

		result = mDevice->CreateBuffer(&bufferDesc, &subData, mBuffer.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("faield to create light buffer");
			return;
		}
		
	}
	Light::~Light()
	{
	}
	void Light::awake()
	{
		mTransform = mRoot->GetComponent<Transform>();
		

	}

	void Light::start()
	{
		

	}

	void Light::update(float delta)
	{
		static Directional directionLight;
		static Point pointLight;

		if (mType == LIGHT_DIRECTIONAL)
		{
			XMVECTOR rotation = mTransform->GetRotation();

			XMStoreFloat3(&directionLight.Direction, rotation);
			mContext->UpdateSubresource(mBuffer.Get(), 0, nullptr, &directionLight, 0, 0);
		}
		else if (mType == LIGHT_POINT)
		{
			XMVECTOR position = mTransform->GetPosition();

			XMStoreFloat3(&pointLight.Position, position);
			mContext->UpdateSubresource(mBuffer.Get(), 0, nullptr, &pointLight, 0, 0);
		}

	}

	void Light::render(float delta)
	{
	}

	void Light::release()
	{
	}


}
