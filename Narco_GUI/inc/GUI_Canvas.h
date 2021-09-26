#pragma once

#include<Common.h>
#include"GUI_Frame.h"


namespace NARCO
{
	class GUI_Canvas : public IGUI
	{
		PUBLIC_API NARCO_API GUI_Canvas(HWND winHandle, HINSTANCE handleInst, ID3D11Device* device, ID3D11DeviceContext* context);
		PUBLIC_API NARCO_API ~GUI_Canvas();

		PUBLIC_API NARCO_API virtual void Start() override;
		PUBLIC_API NARCO_API virtual void Update() override;
		PUBLIC_API NARCO_API virtual void Draw() override;
		PUBLIC_API NARCO_API virtual void End() override;

		PUBLIC_API NARCO_API void AddFrame(GUI_Frame* frame);
		PUBLIC_API NARCO_API GUI_Frame* GetFrame(unsigned int i);

		PRIVATE_PROPERTY HWND mWinHandle;
		PRIVATE_PROPERTY HINSTANCE mHandleInstance;
		PRIVATE_PROPERTY ID3D11Device* mDevice;
		PRIVATE_PROPERTY ID3D11DeviceContext* mContext;
	

		PRIVATE_PROPERTY std::vector<GUI_Frame*> mGuiFrames;
	};
}