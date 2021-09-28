#pragma once

#include<Common.h>
#include<AssetManager.h>

#include"IGUI.h"
#include"ImGui_Texture.h"

namespace NARCO
{
	class GUI_AssetManager : public IGUI
	{
		PUBLIC_API NARCO_API GUI_AssetManager(AssetManager* manager, ID3D11Device* device);
		PUBLIC_API NARCO_API virtual ~GUI_AssetManager();

		PUBLIC_API virtual NARCO_API void Start() override;
		PUBLIC_API virtual NARCO_API void Update() override;
		PUBLIC_API virtual NARCO_API void Draw() override;
		PUBLIC_API virtual NARCO_API void End() override;

		PRIVATE_PROPERTY std::map<long long, ImGui_Texture*> mThumbnails;

		PRIVATE_PROPERTY ID3D11Device* mDevice;
		PRIVATE_PROPERTY AssetManager* mManager;

		PRIVATE_PROPERTY std::string mSearchPath;

	};
}