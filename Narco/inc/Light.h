#pragma once
#include<Common.h>
#include"Component.h"
#include"Transform.h"
#include"GameObject.h"

namespace NARCO
{
	enum eLightType
	{
		LIGHT_POINT,
		LIGHT_DIRECTIONAL,
	};

	enum eLightMode
	{
		LIGHT_REALTIME,
		LIGHT_BAKE,
	};

	class Light : public Component
	{
	public:
		Light(eLightType type, eLightMode mode);
		~Light();
	private:

		Transform* mTransform;

		eLightType mType;
		eLightMode mMode;

		float mIntensity;

		// Component을(를) 통해 상속됨
		virtual void awake() override;

		virtual void start() override;

		virtual void update(float delta) override;

		virtual void render(float delta) override;

		virtual void release() override;

	};
}