#pragma once
#include"Common.h"
#include"D3DHardware.h"
#include"D3DDisplay.h"

#include"MeshLoader.h"
#include"Scene.h"

#include"GBuffer.h"

#include"Renderer.h"

#include"AssetManager.h"

#include<GUI_Bundle.h>
#pragma comment(lib, "Narco_GUI.lib")
#pragma comment(lib, "libDX11.lib")

#include<Windows.h>

namespace NARCO
{
#define NARCO_INIT_APP_WIDTH 1280
#define NARCO_INIT_APP_HEIGHT 720

	class Narco_Core
	{
	public:

		Narco_Core(HWND windowHandle, HINSTANCE handleInst);

		void Init();
		void Update(float delta);
		void Render(float delta);
		void Release();


	private:
		void clearScreen(const float* clearColor);

		Scene* mSelectedScene;

		HWND mWindowHandle;
		HINSTANCE mHandleInstance;

		D3DHardware* mHardware;
		D3DDisplay* mDisplay;

		D3DDepthStencil* mDepth;

		GBuffer* mGBuffer;

		GUI_Canvas* mMainCanvas;
		
	};

}