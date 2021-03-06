#include"GUI_FileSlot.h"
#include<direct.h>
#include<Common.h>
#include<Asset.h>


namespace NARCO
{
	using namespace std::filesystem;

	NARCO_API void GUI_FileSlot::Update()
	{
		ImGui::Text("GUI_FileSlot");

		const char* path = mFilePath.c_str();

		ImGui::Text("File : "); ImGui::SameLine(); ImGui::Text(path);
		if (ImGui::Button("Load") == true)
		{
			mbOpenBrowser = !mbOpenBrowser;
		}

		if (mImage != nullptr)
		{
			ImGui::NewLine();
			ImGui::Image(mImage->GetSRV(), ImVec2(100, 100));
		}

		if (mbOpenBrowser == true)
		{
			fileBrowse();
		}
	}
	NARCO_API void GUI_FileSlot::Draw()
	{
	}

	NARCO_API GUI_FileSlot::GUI_FileSlot(eAssetType type, ID3D11Device* device, const char* initPath)
		: mSelectedPath(initPath), mFilePath(""), mType(type), mDevice(device), mItemIndex(0), mBrowserDropSelect(-1), mBrowserDropItr(0)
	{
		auto lambdaGetDir = []() -> std::string
		{
			char tmp[256];
			_getcwd(tmp, 256);

			return std::string(tmp);
		};

		EDITOR_ASSET_PATH = lambdaGetDir() + "/x64/Debug/resources/";

		if (mSelectedPath == "")
		{
			mSelectedPath = EDITOR_ASSET_PATH;
		}
	}

	NARCO_API GUI_FileSlot::~GUI_FileSlot()
	{
	}

	NARCO_API void GUI_FileSlot::Start()
	{


	}
	NARCO_API void GUI_FileSlot::End()
	{
	}
	NARCO_API void GUI_FileSlot::loadFile(ID3D11Device* device)
	{
		if (mFilePath == "")
		{
			Debug::Log("invalid path. => \"" + mFilePath + '\"');
			return;
		}

		if (mType == ASSET_IMAGE)
		{
			ComPtr<ID3D11ShaderResourceView> view;
			ComPtr<ID3D11Texture2D> texture;

			ImGui_Texture* tex = new ImGui_Texture(device, mFilePath.c_str());
			
			if (tex->GetSRV() == nullptr)
			{
				Debug::Log("invalid file pointer.");
				return;
			}

			if (mImage != nullptr)
			{
				delete mImage;
			}
			
			mImage = tex;
		}
	}
	NARCO_API bool GUI_FileSlot::fileBrowse()
	{
		ImGui::Begin(mSlotName.c_str(), &mbOpenBrowser);

		static auto driveVector = GetDriveStrings();
		static unsigned int driveCount = driveVector.size();
		static const char** drives = new const char* [driveCount];

		static ImGuiIO& io = ImGui::GetIO();

		static auto lamda_bDoubleClick = []() -> bool
		{
			for (unsigned int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
			{
				if (ImGui::IsMouseDoubleClicked(i))
				{
					return true;
				}
			}
			return false;
		};

		for (unsigned int i = 0; i < driveCount; i++)
		{
			drives[i] = driveVector[i].c_str();
		}

		if (ImGui::BeginCombo("", drives[mItemIndex]))
		{
			for (unsigned int i = 0; i < driveVector.size(); i++)
			{
				static bool bSelected = mItemIndex == i;

				if (ImGui::Selectable(drives[i], bSelected))
				{
					mItemIndex = i;
				}

				if (bSelected)
				{
					ImGui::SetItemDefaultFocus();
					mSelectedPath = drives[mItemIndex];

				}

			}
			ImGui::EndCombo();


		}

		std::stringstream str = std::stringstream();



		ImGui::SameLine();
		if (ImGui::ArrowButton("Browser_UpperDirectory", ImGuiDir_::ImGuiDir_Up))
		{
			if (mSelectedPath.size() > 3)
			{
				if (mSelectedPath.find_last_of('/') == std::string::npos)
				{
					mSelectedPath = mSelectedPath.substr(0, mSelectedPath.find_last_of('\\'));
					if (mSelectedPath.find_last_of('/') + 1 != std::string::npos + 1)
					{
						mSelectedPath = mSelectedPath.substr(0, mSelectedPath.find_last_of('/') + 1);
					}
					else
					{
						mSelectedPath = mSelectedPath.substr(0, mSelectedPath.find_last_of('\\') + 1);
					}
				}
				else
				{
					mSelectedPath = mSelectedPath.substr(0, mSelectedPath.find_last_of('/'));
					if (mSelectedPath.find_last_of('/') + 1 != std::string::npos + 1)
					{
						mSelectedPath = mSelectedPath.substr(0, mSelectedPath.find_last_of('/') + 1);
					}
					else
					{
						mSelectedPath = mSelectedPath.substr(0, mSelectedPath.find_last_of('\\') + 1);
					}
				}
				
				std::cout << mSelectedPath << std::endl;
			}

		}
		ImGui::Separator();
		
		for (directory_iterator itr(mSelectedPath); itr != directory_iterator(); ++itr, ++mBrowserDropItr)
		{
			static ImVec2 itemRectSize;
			static ImVec2 itemRectMin;
			static ImVec2 itemRectMax;
			str.str("");

			str << itr->path().filename().string();
			std::string labelString = str.str();
			const char* label = labelString.c_str();

			int attribute = GetFileAttributes(itr->path().c_str());

			if (attribute & FILE_ATTRIBUTE_HIDDEN)
			{
				continue;
			}

			if (ImGui::Selectable(label, mBrowserDropSelect == mBrowserDropItr))
			{
				mBrowserDropSelect = mBrowserDropItr;

				itemRectSize = ImGui::GetItemRectSize();
				itemRectMin = ImGui::GetItemRectMin();
				itemRectMax = ImGui::GetItemRectMax();

			}

			if (mBrowserDropSelect == mBrowserDropItr)
			{
				bool bDirectory = itr->is_directory();
				bool bExists = itr->exists();
				bool bHovering = ImGui::IsMouseHoveringRect(itemRectMin, itemRectMax);
				bool bDoubleClick = lamda_bDoubleClick();
				bool bFile = itr->is_character_file();
				std::filesystem::file_status pathStatus = status(itr->path());
				std::filesystem::perms permission = pathStatus.permissions();

				if (bDirectory && bExists && bHovering && bDoubleClick)
				{

					if (((uint)permission & (uint)perms::group_read) == false &&
						((uint)permission & (uint)perms::others_read) == false &&
						((uint)permission & (uint)perms::owner_read) == false)
					{
						Debug::Log(std::to_string((uint)permission));
						Debug::Log("Permission failed!");
						continue;
					}
					mSelectedPath += label;
					mSelectedPath += '/';
				}

				else if (!bDirectory && bExists && bHovering && bDoubleClick)
				{
					std::string temp = mSelectedPath + label;
					std::string extension = temp.substr(temp.find_last_of('.')) + "\"";
					
					auto result = EXTENSION_MAP.find(extension);

					if (result == EXTENSION_MAP.end())
					{
						Debug::Log("Not supported");
						continue;
					}
					
					eAssetType type = result->second; //= Asset::DetermineType(extension);

					if (type == mType)
					{
						mFilePath = mSelectedPath;
						mFilePath += label;
#ifdef _DEBUG
						std::cout << "Selected File : " << mFilePath << std::endl;
#endif
						loadFile(mDevice);
					}


				}
			}
		}

		mBrowserDropItr = 0;
		
		ImGui::End();
		return true;
	}
	NARCO_API std::vector<std::string> GUI_FileSlot::GetDriveStrings()
	{
		std::vector<std::string> strings;

		char driveChar[] = { "A:/" };

		DWORD driveMask = GetLogicalDrives();
		if (driveMask == 0)
		{
			Debug::Throw("No drive has been detected.");
			throw std::exception("No drive has been detected.");
		}
		else
		{
			while (driveMask)
			{
				++driveChar[0];
				driveMask >>= 1;

				if (driveMask & 1)
				{
					strings.emplace_back(driveChar);
				}

			}
		}


		return strings;

	}
}