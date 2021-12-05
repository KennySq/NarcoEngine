#pragma once
#include<Common.h>

#include"Transform.h"
#include"Component.h"

namespace NARCO
{
	enum eLightMode
	{
		LIGHT_REALTIME,
		LIGHT_BAKE,
	};

	enum eLightType
	{
		LIGHT_DIRECTIONAL,
		LIGHT_POINT,
	};


	class Light : public Component
	{
	public:
		struct DirectionalLight
		{
			XMFLOAT3 Direction;
			float Intensity;
			XMFLOAT4 Color;

			bool operator==(const DirectionalLight& other)
			{
				XMVECTOR dir = XMLoadFloat3(&Direction), otherDir = XMLoadFloat3(&other.Direction);
				XMVECTOR color = XMLoadFloat4(&Color), otherColor = XMLoadFloat4(&other.Color);

				return XMVector3Equal(dir, otherDir) && XMVector3Equal(color, otherColor) && Intensity == other.Intensity;
			}
		};

		struct PointLight
		{
			XMFLOAT3 Position;
			float Intensity;
			XMFLOAT4 Color;

			bool operator==(const PointLight& other)
			{
				XMVECTOR pos = XMLoadFloat3(&Position), otherPos = XMLoadFloat3(&other.Position);
				XMVECTOR color = XMLoadFloat4(&Color), otherColor = XMLoadFloat4(&other.Color);

				return XMVector3Equal(pos, otherPos) && XMVector3Equal(color, otherColor) && Intensity == other.Intensity;
			}
		};

		eLightType GetType() const { return mType; }
		eLightMode GetMode() const { return mMode; }

		const DirectionalLight& GetDirectional() const { return mDirectional; }
		const PointLight& GetPoint() const { return mPoint; }

	private:
		bool compare();
		void notify();

		static uint LightID;
		const Transform* mTransform;

		eLightType mType;
		eLightMode mMode;

		DirectionalLight mDirectional;
		DirectionalLight mPrevDirectional;

		PointLight mPoint;
		PointLight mPrevPoint;

		float mIntensity;
		XMFLOAT4 mColor;
		

		uint mID;

		// Component을(를) 통해 상속됨
		virtual void awake() override;

		virtual void start() override;

		virtual void update(float delta) override;

		virtual void render(float delta) override;

		virtual void release() override;

	};
}
