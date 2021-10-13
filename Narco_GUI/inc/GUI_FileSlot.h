#pragma once
#include"IGUI.h"

#include"ImGui_Texture.h"

namespace NARCO
{
	enum eAssetType;
	class GUI_FileSlot : public IGUI
	{
		PUBLIC_API NARCO_API GUI_FileSlot(eAssetType type, ID3D11Device* device);
		PUBLIC_API NARCO_API ~GUI_FileSlot();

		PUBLIC_API NARCO_API ImGui_Texture* GetImage() const { return mImage; }

		PUBLIC_API NARCO_API virtual void Start() override;
		PUBLIC_API NARCO_API virtual void Update() override;
		PUBLIC_API NARCO_API virtual void Draw() override;
		PUBLIC_API NARCO_API virtual void End() override;
		
		PRIVATE_API NARCO_API void loadFile(ID3D11Device* device);
		PRIVATE_API NARCO_API bool fileBrowse();

		PRIVATE_API NARCO_API std::vector<std::string> GetDriveStrings();

		PRIVATE_PROPERTY std::string mFilePath;
		PRIVATE_PROPERTY bool mbOpenBrowser = false;
		PRIVATE_PROPERTY std::string mSelectedPath = "C:/";

		PRIVATE_PROPERTY eAssetType mType;

		PRIVATE_PROPERTY ImGui_Texture* mImage = nullptr;
		PRIVATE_PROPERTY ID3D11Device* mDevice;

		PRIVATE_PROPERTY int mItemIndex = 0;
		PRIVATE_PROPERTY int mBrowserDropSelect = -1;
		PRIVATE_PROPERTY int mBrowserDropItr = 0;


		
	};
}