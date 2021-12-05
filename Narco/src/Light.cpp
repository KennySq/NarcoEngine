#include "../inc/Light.h"
#include "../inc/GameObject.h"
#include "../inc/Scene.h"
namespace NARCO
{
	uint Light::LightID = 0;

	bool Light::compare()
	{
		if (mType == LIGHT_DIRECTIONAL)
		{
			if ((mDirectional == mPrevDirectional))
			{
				return true;
			}
		}
		else if (mType == LIGHT_POINT)
		{
			if ((mPoint == mPrevPoint))
			{
				return true;
			}
		}

		return false;
	}

	void Light::notify()
	{
		Scene* scene = mRoot->GetScene();

	}

	void Light::awake()
	{
		mTransform = mRoot->GetComponent<Transform>();
		mID = LightID++;
	}
	void Light::start()
	{
	}
	void Light::update(float delta)
	{
		Scene* scene = mRoot->GetScene();
		if (mType == LIGHT_DIRECTIONAL)
		{
			XMStoreFloat3(&mDirectional.Direction, mTransform->GetRotation());
			mDirectional.Color = mColor;
			mDirectional.Intensity = mIntensity;

			reinterpret_cast<void*>(&mDirectional);
		}
		else if (mType == LIGHT_POINT)
		{
			XMStoreFloat3(&mPoint.Position, mTransform->GetPosition());
			mPoint.Color = mColor;
			mPoint.Intensity = mIntensity;

			reinterpret_cast<void*>(&mPoint);
		}

		if (compare() == false)
		{
			scene->UpdateLight();
		}

		mPrevDirectional = mDirectional;
		mPrevPoint = mPoint;

	}
	void Light::render(float delta)
	{
	}
	void Light::release()
	{
	}
}