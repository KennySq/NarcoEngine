#pragma once

#include<Common.h>

#include<imgui.h>
#include<imgui_impl_win32.h>
#include<imgui_impl_dx11.h>

namespace NARCO
{
	class IGUI
	{
		PUBLIC_API NARCO_API virtual void Start() = 0;
		PUBLIC_API NARCO_API virtual void Update() = 0;
		PUBLIC_API NARCO_API virtual void Draw() = 0;
		PUBLIC_API NARCO_API virtual void End() = 0;
	};
}