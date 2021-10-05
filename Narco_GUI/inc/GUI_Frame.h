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

		PUBLIC_API NARCO_API void AddGUI(const char* name, IGUI* gui);
		PUBLIC_API NARCO_API void AddGUI(GUI_Canvas*) = delete;
		PUBLIC_API NARCO_API void AddGUI(GUI_Frame*) = delete;

		PUBLIC_API NARCO_API IGUI* GetGUI(const char* name) const {
			long long hash = MakeHash(name);
			auto result = mGuiMap.find(hash);

			if (result != mGuiMap.end())
			{
				return result->second;
			}

			Debug::Log(std::string(name) + " is not found");
			return nullptr;
		}
		PRIVATE_PROPERTY std::map<long long, IGUI*> mGuiMap;

		PRIVATE_PROPERTY std::vector<IGUI*> mGuiObjects;
		PRIVATE_PROPERTY const char* mName;
		PRIVATE_PROPERTY unsigned int mWidth;
		PRIVATE_PROPERTY unsigned int mHeight;
		PRIVATE_PROPERTY ImGuiWindowFlags mWindowFlags;
	};
}