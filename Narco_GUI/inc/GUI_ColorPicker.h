#pragma once
#include<Common.h>
#include"IGUI.h"

namespace NARCO
{
	class GUI_ColorPicker : public IGUI
	{
		PUBLIC_API NARCO_API virtual void Start() override;
		PUBLIC_API NARCO_API virtual void Update() override;
		PUBLIC_API NARCO_API virtual void Draw() override;
		PUBLIC_API NARCO_API virtual void End() override;

		const float* GetColor4() const { return mColor; }

		PRIVATE_PROPERTY float mColor[4];
	};
}
