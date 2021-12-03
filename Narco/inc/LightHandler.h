#pragma once
#include<Common.h>

#include"Super.h"
#include"Light.h"

using namespace DirectX;

namespace NARCO
{
	class Scene;
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
		
		void OnChangeScene(Scene* newScene)
		{
			mCurrentScene = newScene;
		}

		Scene* GetScene() { return mCurrentScene; }
	private:

		static LightHandler* mInstance;

		ComPtr<ID3D11ClassLinkage> mLinkage;

		Scene* mCurrentScene;

		const uint MAX_DIRECTIONAL_LIGHT = 1024;
		const uint MAX_POINT_LIGHT = 1024;
	
	};
}