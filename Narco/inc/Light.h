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

		Light(eLightType type, eLightMode mode);
		~Light();
	
		eLightMode GetMode() const { return mMode; }
		eLightType GetType() const { return mType; }
	protected:
		Transform* mTransform;

		ComPtr<ID3D11ClassLinkage> mLinkage;

		eLightType mType;
		eLightMode mMode;

		uint mSize;

		// Component을(를) 통해 상속됨
		virtual void awake() override;

		virtual void start() override;

		virtual void update(float delta) override;

		virtual void render(float delta) override;

		virtual void release() override;

	};
}