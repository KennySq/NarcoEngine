#include"GUI_Frame.h"

namespace NARCO
{
	NARCO_API GUI_Frame::GUI_Frame(const char* name, unsigned int width, unsigned int height, ImGuiWindowFlags winFlags)
		: mName(name), mWidth(width), mHeight(height), mWindowFlags(winFlags)
	{
		
	}
	NARCO_API void GUI_Frame::Start()
	{
		unsigned int guiCount = mGuiObjects.size();

		for (unsigned int i = 0; i < guiCount; i++)
		{
			mGuiObjects[i]->Start();
		}
	}
	NARCO_API void GUI_Frame::Update()
	{

		ImGui::Begin(mName,nullptr, mWindowFlags);

		unsigned int guiCount = mGuiObjects.size();

		for (unsigned int i = 0; i < guiCount; i++)
		{
			mGuiObjects[i]->Update();
		}
		ImGui::End();

	}
	NARCO_API void GUI_Frame::Draw()
	{


	}
	NARCO_API void GUI_Frame::End()
	{
		unsigned int guiCount = mGuiObjects.size();

		for (unsigned int i = 0; i < guiCount; i++)
		{
			mGuiObjects[i]->End();
		}

	}

	NARCO_API void GUI_Frame::AddGUI(const char* name, IGUI* gui)
	{
		mGuiMap.insert_or_assign(MakeHash(name), gui);
		mGuiObjects.emplace_back(gui);

	}
}