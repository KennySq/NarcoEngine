
#include "inc/Light.h"
#include"Transform.h"
#include"GameObject.h"
#include"inc/Scene.h"

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


	void DirectionalLight::awake()
	{
		static LightHandler* lightHandler = LightHandler::GetInstance();
		mTransform = mRoot->GetComponent<Transform>();
		Scene* scene = lightHandler->GetScene();

		scene->AddLight(this);
	}

	void DirectionalLight::start()
	{
	}

	void DirectionalLight::update(float delta)
	{
	}

	void DirectionalLight::render(float delta)
	{
	}

	void DirectionalLight::release()
	{
	}

	void PointLight::awake()
	{
	}

	void PointLight::start()
	{
	}

	void PointLight::update(float delta)
	{
	}

	void PointLight::render(float delta)
	{
	}

	void PointLight::release()
	{
	}

}
