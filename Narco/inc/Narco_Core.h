#pragma once
#include"Common.h"
#include"D3DHardware.h"
#include"D3DDisplay.h"

#include"MeshLoader.h"
#include"Scene.h"

#include"GBuffer.h"

#include"AssetManager.h"

#include"Prefab.h"
#include"Shiba.h"


#include"SVO.h"

#include<GUI_Bundle.h>
#pragma comment(lib, "Narco_GUI.lib")
#pragma comment(lib, "libDX11.lib")

#include<Windows.h>

namespace NARCO
{
#define NARCO_INIT_APP_WIDTH 1280
#define NARCO_INIT_APP_HEIGHT 720

	class Narco_Deferred_Legacy
	{
	public:

		Narco_Deferred_Legacy(HWND windowHandle, HINSTANCE handleInst);

		void PreInit();
		void Init();
		void Update(float delta);
		void Render(float delta);
		void Release();

		GBuffer* GetGBuffer() const { return mGBuffer; }

		void BoundBackBuffer(ID3D11DeviceContext* context) const
		{
			mGBuffer->Unbound(context);

			static ID3D11RenderTargetView* backBuffer[] = { mDisplay->GetRenderTargetView() };
			context->OMSetRenderTargets(1, backBuffer, nullptr);


		}

		void BoundGBuffer(ID3D11DeviceContext* context) const
		{
			mGBuffer->Bound(context);
		}

	private:
		void clearScreen(const float* clearColor);
		void drawScreen();

		Scene* mSelectedScene;

		HWND mWindowHandle;
		HINSTANCE mHandleInstance;

		D3DHardware* mHardware;
		D3DDisplay* mDisplay;

		D3DDepthStencil* mDepth;

		GBuffer* mGBuffer;

		GUI_Canvas* mMainCanvas;

		SVO* mVoxelOctree;
		
	};

}