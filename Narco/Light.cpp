
#include "inc/Light.h"

namespace NARCO
{
	Light::Light(eLightType type, eLightMode mode)
		: mMode(mode), mType(type)
	{
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
	}

	void Light::render(float delta)
	{
	}

	void Light::release()
	{
	}


}
