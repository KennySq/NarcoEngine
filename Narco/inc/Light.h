#pragma once
#include<Common.h>
#include"Component.h"

namespace NARCO
{
	class Transform;
	class GameObject;
	
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

	};

	class DirectionalLight : public Light
	{
	public:
		struct Raw
		{
			XMFLOAT3 Direction;
			float Intensity;

			XMFLOAT4 Color;
		};

		DirectionalLight()
			: Light(LIGHT_DIRECTIONAL, LIGHT_REALTIME)
		{
			XMVECTOR direction = XMVectorSet(500, 500, -500, 1.0f) - XMVectorSet(0, 0, 0, 1.0f);

			Raw.Intensity = 1.0f;

			XMStoreFloat3(&Raw.Direction, direction);
			XMStoreFloat4(&Raw.Color, Colors::White);
		}

		DirectionalLight(const DirectionalLight& other)
			: Light(other.mType, other.mMode), Raw(other.Raw)
		{

		}

		virtual void awake() override;
		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;

		~DirectionalLight() {}
		
		Raw Raw;
	};

	class PointLight : public Light
	{
	public:
		struct Raw
		{
			XMFLOAT3 Position;
			float Intensity;

			XMFLOAT4 Color;
		};

		PointLight()
			: Light(LIGHT_POINT, LIGHT_REALTIME)
		{
			XMVECTOR position = XMVectorSet(0, 0, 0, 1.0f);
			
			XMStoreFloat3(&Raw.Position, position);
			XMStoreFloat4(&Raw.Color, Colors::White);
		}
		
		PointLight(const PointLight& other)
			: Light(other.mType, other.mMode), Raw(other.Raw)
		{

		}
		~PointLight() {}

		virtual void awake() override;
		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;

		Raw Raw;

	};
}