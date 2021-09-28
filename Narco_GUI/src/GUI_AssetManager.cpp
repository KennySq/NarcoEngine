#include"../inc/GUI_AssetManager.h"

using namespace std::filesystem;

namespace NARCO
{
	GUI_AssetManager::GUI_AssetManager(AssetManager* manager, ID3D11Device* device)
		: mManager(manager), mDevice(device)
	{
		auto assets = mManager->GetPageAssets();

		auto systemAssets = mManager->GetSystemAssets();
		
		ImGui_Texture* folderTex = new ImGui_Texture(device, "C:/Users/odess/Desktop/Projects/NarcoEngine/Narco/x64/Debug/resources/textures/system/icon_folder.png");
		ImGui_Texture* imageTex = new ImGui_Texture(device, "C:/Users/odess/Desktop/Projects/NarcoEngine/Narco/x64/Debug/resources/textures/system/icon_image.png");
		for (auto& i : assets)
		{
			long long hash = i.first;
			eAssetType type = i.second->GetAssetType();
			std::string strPath = i.second->GetPath();

			int firstIndex = strPath.find_first_of('\"') + 1;
			int lastIndex = strPath.find_last_of('\"');

			auto first = strPath.substr(firstIndex);
			lastIndex = first.find_last_of('\"');

			auto last = first.substr(0, lastIndex);

			const char* path = last.c_str();

#ifdef _DEBUG
			std::cout << path << std::endl;
#endif

			if (type == eAssetType::ASSET_IMAGE)
			{
				mThumbnails.insert_or_assign(hash, new ImGui_Texture(device, path));
			}

			else if (type == eAssetType::ASSET_DIRECTORY)
			{
				mThumbnails.insert_or_assign(hash, folderTex);
			}
		}

	}
	GUI_AssetManager::~GUI_AssetManager()
	{
	}
	NARCO_API void GUI_AssetManager::Start()
	{
		return void();
	}
	NARCO_API void GUI_AssetManager::Update()
	{
		path searchPath(mSearchPath);
		auto assets = mManager->GetPageAssets();
		
		ImGui::Text(mManager->GetCurrentPath().c_str());
		ImGui::Separator();
		for (auto& i : mThumbnails)
		{
			std::string fileName = i.second->GetFileName();

			fileName = fileName.replace(fileName.begin(), fileName.begin() + 1, "");
			if (fileName.size() > 9)
			{
				fileName = fileName.replace(fileName.begin() + 9, fileName.end(), "...");
			}

			ImGui::BeginGroup();
			ImGui::ImageButton(i.second->GetSRV(), ImVec2(64, 64),ImVec2(0,0), ImVec2(1,1), 10);
			//ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 64);
			
			ImGui::Text(fileName.c_str());

			ImGui::EndGroup();

			ImGui::SameLine();
		}

		return;
	}
	NARCO_API void GUI_AssetManager::Draw()
	{
		return void();
	}
	NARCO_API void GUI_AssetManager::End()
	{
		return void();
	}
}