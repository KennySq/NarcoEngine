#pragma once
#include"IGUI.h"
#include<Asset.h>

namespace NARCO
{

	class GUI_FileSlot : public IGUI
	{
		PUBLIC_API NARCO_API GUI_FileSlot(eAssetType type);
		PUBLIC_API NARCO_API ~GUI_FileSlot();

		PUBLIC_API NARCO_API virtual void Start() override;
		PUBLIC_API NARCO_API virtual void Update() override;
		PUBLIC_API NARCO_API virtual void Draw() override;
		PUBLIC_API NARCO_API virtual void End() override;
		
		PRIVATE_API NARCO_API void loadFile(const char* path);
		PRIVATE_API NARCO_API bool fileBrowse();

		PRIVATE_API NARCO_API std::vector<std::string> GetDriveStrings();

		PRIVATE_PROPERTY std::string mFilePath;
		PRIVATE_PROPERTY bool mbOpenBrowser = false;
		PRIVATE_PROPERTY std::string mSelectedPath = "C:/";

		PRIVATE_PROPERTY eAssetType mType;
	};
}