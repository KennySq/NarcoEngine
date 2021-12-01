#pragma once
#include<Common.h>

#include"Super.h"
#include"Light.h"

using namespace DirectX;

namespace NARCO
{
	class PointLight : public Light
	{
	public:
		XMFLOAT3 Diffuse(XMFLOAT3 normal);
		XMFLOAT3 Specular(XMFLOAT3 normal, float power);
	private:
		XMFLOAT3 mPosition;
		float mIntensity;
	};

	class DirectionalLight : public Light
	{
	public:
		XMFLOAT3 Diffuse(XMFLOAT3 normal);
		XMFLOAT3 Specular(XMFLOAT3 normal, float power);

		virtual void update(float delta) override;

	private:
		XMFLOAT3 mDirection;
		float mIntensity;
		XMFLOAT4 mColor;


	};

	class LightHandler : public Super
	{
	public:
		LightHandler();
		~LightHandler();

		ID3D11ClassLinkage* GetLinkage() const { return mLinkage.Get(); }

		static LightHandler* GetInstance()
		{
			if (mInstance == nullptr)
			{
				mInstance = new LightHandler();
			}

			return mInstance;
		}

	private:
		static LightHandler* mInstance;

		ComPtr<ID3D11ClassLinkage> mLinkage;
	
	
	};
}