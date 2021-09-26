#pragma once
#include<Common.h>
#include"IGUI.h"
#include"GUI_Frame.h"

namespace NARCO
{
	class GUI_Canvas;
	class GUI_Frame : public IGUI
	{
		PUBLIC_API NARCO_API GUI_Frame(const char* name, unsigned int width, unsigned int height, ImGuiWindowFlags winFlags);

		PUBLIC_API NARCO_API virtual void Start() override;
		PUBLIC_API NARCO_API virtual void Update() override;
		PUBLIC_API NARCO_API virtual void Draw() override;
		PUBLIC_API NARCO_API virtual void End() override;

		PUBLIC_API NARCO_API void AddGUI(IGUI* gui);
		PUBLIC_API NARCO_API void AddGUI(GUI_Canvas*) = delete;
		PUBLIC_API NARCO_API void AddGUI(GUI_Frame*) = delete;

		PRIVATE_PROPERTY std::vector<IGUI*> mGuiObjects;
		PRIVATE_PROPERTY const char* mName;
		PRIVATE_PROPERTY unsigned int mWidth;
		PRIVATE_PROPERTY unsigned int mHeight;
		PRIVATE_PROPERTY ImGuiWindowFlags mWindowFlags;
	};
}