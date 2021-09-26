#include"GUI_FileSlot.h"

namespace NARCO
{
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


		DWORD dirves = GetLogicalDrives();

		const char* drives[] = {"drive1", "drive2", "drive3"};

		static int itemIndex = 0;
		if (ImGui::BeginCombo("Dirve", drives[itemIndex]))
		{
			for (unsigned int i = 0; i < ARRAYSIZE(drives); i++)
			{
				const bool bSelected = itemIndex == i;

				if (ImGui::Selectable(drives[i], bSelected))
				{
					itemIndex = i;
				}

				if (bSelected)
				{
					ImGui::SetItemDefaultFocus();
				}

			}
			ImGui::EndCombo();
		}
		//ImGui::Combo("")

		ImGui::End();
		//ImGui::EndCombo();

		return true;
	}
}