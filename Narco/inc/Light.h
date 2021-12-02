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

		// Component을(를) 통해 상속됨
		virtual void awake() override;
		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;

	};

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(XMVECTOR direction, float intensity, XMVECTOR color, eLightType type, eLightMode mode)
			: Light(type, mode), Intensity(intensity)
		{
			XMStoreFloat3(&Direction, direction);
			XMStoreFloat4(&Color, color);
		}

		DirectionalLight(const DirectionalLight& other)
			: Light(other.mType, other.mMode), Intensity(other.Intensity), Direction(other.Direction), Color(other.Color)
		{

		}

		~DirectionalLight() {}

		XMFLOAT3 Direction;
		float Intensity;

		XMFLOAT4 Color;
	};

	class PointLight : public Light
	{
	public:
		PointLight(XMVECTOR position, float intensity, XMVECTOR color, eLightType type, eLightMode mode)
			: Light(type, mode), Intensity(intensity)
		{
			XMStoreFloat3(&Position, position);
			XMStoreFloat4(&Color, color);
		}
		
		PointLight(const PointLight& other)
			: Light(other.mType, other.mMode), Intensity(other.Intensity), Position(other.Position), Color(other.Color)
		{

		}
		~PointLight() {}

		XMFLOAT3 Position;
		float Intensity;

		XMFLOAT4 Color;
	};
}