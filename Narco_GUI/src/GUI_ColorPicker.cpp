#include"GUI_ColorPicker.h"


namespace NARCO
{
	NARCO_API void GUI_ColorPicker::Start()
	{
		mColor[0] = 0;
		mColor[1] = 0;
		mColor[2] = 0;
		mColor[3] = 0;
	}

	NARCO_API void GUI_ColorPicker::Update()
	{
		ImGui::ColorPicker4("Color4", mColor, ImGuiColorEditFlags_::ImGuiColorEditFlags_DisplayRGB);
	}

	NARCO_API void GUI_ColorPicker::Draw()
	{
	}

	NARCO_API void GUI_ColorPicker::End()
	{
	}

}
