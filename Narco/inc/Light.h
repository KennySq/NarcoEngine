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
		struct Point
		{
			DirectX::XMFLOAT3 Position;
			float Intesnity;
		};

		struct Directional
		{
			DirectX::XMFLOAT3 Direction;
			float Intensity;
		};

	public:
		Light(eLightType type, eLightMode mode);
		~Light();
	private:
		ComPtr<ID3D11Buffer> mBuffer;
		Transform* mTransform;

		eLightType mType;
		eLightMode mMode;

		// Component을(를) 통해 상속됨
		virtual void awake() override;

		virtual void start() override;

		virtual void update(float delta) override;

		virtual void render(float delta) override;

		virtual void release() override;

	};
}