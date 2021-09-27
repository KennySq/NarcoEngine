#include"GUI_FileSlot.h"

namespace NARCO
{
	using namespace std::filesystem;

	NARCO_API void GUI_FileSlot::Update()
	{
		ImGui::Text("GUI_FileSlot");

		const char* path = mPath.c_str();

		ImGui::Text("File : "); ImGui::SameLine(); ImGui::Text(path);
		if (ImGui::Button("Load") == true)
		{
			mbOpenBrowser = !mbOpenBrowser;
		}

		if (mbOpenBrowser == true)
		{
			fileBrowse();



		}
	}
	NARCO_API void GUI_FileSlot::Draw()
	{
	}

	NARCO_API GUI_FileSlot::GUI_FileSlot()
		: mPath("")
	{
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
	NARCO_API void GUI_FileSlot::loadFile(const char* path)
	{

	}
	NARCO_API bool GUI_FileSlot::fileBrowse()
	{
		ImGui::Begin("Browser", &mbOpenBrowser);

		static auto driveVector = GetDriveStrings();
		static unsigned int driveCount = driveVector.size();
		static const char** drives = new const char* [driveCount];
		static int itemIndex = 0;


		for (unsigned int i = 0; i < driveCount; i++)
		{
			drives[i] = driveVector[i].c_str();
		}

		mSearchPath = std::string(drives[0]);
		static auto rootIteration = directory_iterator("C:/");

		//#ifdef _DEBUG
		//		
		//		std::cout << "Iterate root path\n";
		//
		//		for(directory_iterator itr("C:/") ;  itr != directory_iterator();itr++)
		//		{
		//
		//			std::cout << "\t¦¦ " << *itr << std::endl;
		//
		//
		//		}
		//#endif



		if (ImGui::BeginCombo("", drives[itemIndex]))
		{
			for (unsigned int i = 0; i < driveVector.size(); i++)
			{
				const bool bSelected = itemIndex == i;

				if (ImGui::Selectable(drives[i], bSelected))
				{
					itemIndex = i;
				}

				if (bSelected)
				{
					ImGui::SetItemDefaultFocus();

					mSearchPath = drives[itemIndex];





				}

			}
			ImGui::EndCombo();


		}

		std::stringstream str = std::stringstream();

		for (directory_iterator itr(mSearchPath); itr != directory_iterator(); itr++)
		{
			if (itr->exists())
			{
				str << itr->path() << '\n';
			}

			const char* label = itr->path().c_str()[0];

			ImGui::Button();

		}


		ImGui::TextWrapped("%s", str.str().c_str());

		//ImGui::Combo("")

		ImGui::End();
		//ImGui::EndCombo();

		return true;
	}
	NARCO_API std::vector<std::string> GUI_FileSlot::GetDriveStrings()
	{
		std::vector<std::string> strings;

		char driveChar[] = { "A:/" };

		DWORD driveMask = GetLogicalDrives();
		if (driveMask == 0)
		{
			ExceptionError(E_FAIL, "No drive has been detected.");
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