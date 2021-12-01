
#include "inc/Light.h"

namespace NARCO
{
	Light::Light(eLightType type, eLightMode mode)
		: mMode(mode), mType(type)
	{
		Directional directional{ {1,0,0}, 1 };
		Point point{ { 1,0,0}, 1 };

		if (type == LIGHT_POINT)
		{
			mSize = sizeof(Point);
		}

		else if (type == LIGHT_DIRECTIONAL)
		{
			mSize = sizeof(Directional);
		}

		HRESULT result = mDevice->CreateClassLinkage(mLinkage.GetAddressOf());
	
		if (result != S_OK)
		{
			Debug::Log("failed to create light class linkage.");
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
			//mContext->UpdateSubresource(mBuffer.Get(), 0, nullptr, &directionLight, 0, 0);
		}
		else if (mType == LIGHT_POINT)
		{
			XMVECTOR position = mTransform->GetPosition();

			XMStoreFloat3(&pointLight.Position, position);
		//	mContext->UpdateSubresource(mBuffer.Get(), 0, nullptr, &pointLight, 0, 0);
		}

	}

	void Light::render(float delta)
	{
	}

	void Light::release()
	{
	}


}
