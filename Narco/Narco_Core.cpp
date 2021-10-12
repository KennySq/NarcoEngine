#include "inc/Narco_Core.h"

#include"inc\Dragon.h"

using namespace NARCO;

namespace NARCO
{
	Narco_Deferred_Legacy::Narco_Deferred_Legacy(HWND windowHandle, HINSTANCE handleInst)
		: mWindowHandle(windowHandle), mHandleInstance(handleInst), mHardware(D3DHardware::GetInstance()),
		mDisplay(new D3DDisplay(windowHandle, mHardware->GetDevice(), NARCO_INIT_APP_WIDTH, NARCO_INIT_APP_HEIGHT))
	{
		ID3D11Device* device = D3DHardware::GetDevice();
		ID3D11DeviceContext* context = D3DHardware::GetImmediateContext();



		unsigned int width = NARCO_INIT_APP_WIDTH;
		unsigned int height = NARCO_INIT_APP_HEIGHT;



		mGBuffer = new GBuffer(device, width, height);



		// 테스트 코드 영역입니다.

		//MeshLoader debugLoader(device);

		//debugLoader.SetPath("C:/Users/odess/Desktop/Projects/NarcoEngine/Narco/x64/Debug/resources/ice_dragon/ice_dragon.fbx");
		//debugLoader.Load();

		//Mesh* mesh_dragon = debugLoader.ConvertMesh();

		mSelectedScene = new Scene("Sample Scene", context, this);

		GameObject* shiba = mSelectedScene->AddGameObject(new Shiba());
		GameObject* dragon = mSelectedScene->AddGameObject(new Dragon());
		GameObject* mainCamera = mSelectedScene->AddGameObject(new GameObject("Main Camera"));

		Camera* mainCam = mainCamera->AddComponent<Camera>();
		mainCam->SetDisplay(mDisplay);
		mainCamera->SetTag("Main Camera");




		mMainCanvas = new GUI_Canvas(mWindowHandle, mHandleInstance, device, context);

		mMainCanvas->AddFrame(new GUI_Frame("File Slot", 300,300, ImGuiWindowFlags_NoResize));
		mMainCanvas->AddFrame(new GUI_Frame("Color Picker", 400,300, ImGuiWindowFlags_NoResize));
		mMainCanvas->AddFrame(new GUI_Frame("Asset Browser", 1100, 400, ImGuiWindowFlags_NoResize));
		mMainCanvas->AddFrame(new GUI_Frame("Inspector", 400, 800, ImGuiFocusedFlags_None));
		mMainCanvas->AddFrame(new GUI_Frame("Material", 400, 800, ImGuiFocusedFlags_None));

	}
	void Narco_Deferred_Legacy::PreInit()
	{
		ID3D11Device* device = D3DHardware::GetDevice();

		mSelectedScene->awake();

		auto frame = mMainCanvas->GetFrame(0);
		auto frame2 = mMainCanvas->GetFrame(1);
		auto frame3 = mMainCanvas->GetFrame(2);
		auto frame4 = mMainCanvas->GetFrame(3);
		auto frame5 = mMainCanvas->GetFrame(4);

		AssetManager* assetManager = new AssetManager("C:/Users/odess/Desktop/Projects/NarcoEngine/Narco/x64/Debug/resources/app/assets");
		
		GameObject* shiba = mSelectedScene->GetGameObject(0);
		Renderer* shibaRenderer = shiba->GetComponent<Renderer>();
		Material* shibaMat = shibaRenderer->GetMaterial();

		GameObject* dragon = mSelectedScene->GetGameObject(1);

		frame->AddGUI("FileSlot_01", new GUI_FileSlot(ASSET_IMAGE, device));
		frame2->AddGUI("ColorPicker_01", new GUI_ColorPicker());
		frame3->AddGUI("AssetBrowser_01", new GUI_AssetManager(assetManager, device));
		frame4->AddGUI("Inspector_01", new GUI_GameObject(dragon));
		

		frame5->AddGUI("Material_01", new GUI_Material(shibaMat, device));

	}
	void Narco_Deferred_Legacy::Init()
	{
		mSelectedScene->start();

		mMainCanvas->Start();

	}
	void Narco_Deferred_Legacy::Update(float delta)
	{
		static ID3D11DeviceContext* context = mHardware->GetImmediateContext();
		static ID3D11RenderTargetView* const * rtv = mGBuffer->GetRenderTargets();
		static ID3D11RenderTargetView* backBuffer[] = { mDisplay->GetRenderTargetView() };
		static ID3D11DepthStencilView* dsv = mGBuffer->GetDepthStencil();
		static D3D11_VIEWPORT viewports[] = { mDisplay->GetMainViewport() };
		static ID3D11ShaderResourceView* nullSrv[6] = { nullptr };

		static GUI_Canvas* canvas = mMainCanvas;
		static GUI_Frame* frame_ColorPicker = canvas->GetFrame(1);
		static GUI_ColorPicker* gui_ColorPicker = static_cast<GUI_ColorPicker*>(frame_ColorPicker->GetGUI("ColorPicker_01"));
		
		clearScreen(gui_ColorPicker->GetColor4());
		mGBuffer->ClearBuffer(context, gui_ColorPicker->GetColor4());

		//context->OMSetRenderTargets(mGBuffer->GetBufferCount(), rtv, dsv);
		//
		//
		context->RSSetViewports(1, viewports);

		mSelectedScene->update(delta);

		context->PSSetShaderResources(0, 6, &nullSrv[0]);

		mGBuffer->DrawScreen(context, backBuffer[0]);
		context->PSSetShaderResources(0, 6, &nullSrv[0]);

		mMainCanvas->Update();

	}
	void Narco_Deferred_Legacy::Render(float delta)
	{
		static ID3D11DeviceContext* context = mHardware->GetImmediateContext();
		static ID3D11RenderTargetView* nullRtv[] = { nullptr };
		static ID3D11ShaderResourceView* nullSrv[6] = { nullptr };

		//context->OMSetRenderTargets(1, nullRtv, nullptr);
		mSelectedScene->render(delta);

		mMainCanvas->Draw();

		context->OMSetRenderTargets(1, nullRtv, nullptr);
		context->PSSetShaderResources(0, 6, &nullSrv[0]);

		

		mDisplay->Present();
		


	}
	void Narco_Deferred_Legacy::Release()
	{
		mSelectedScene->release();

		mMainCanvas->End();

		delete mHardware;
		delete mDisplay;
	}
	void Narco_Deferred_Legacy::clearScreen(const float* clearColor)
	{
		static ID3D11DeviceContext* context = mHardware->GetImmediateContext();
		static ID3D11RenderTargetView* renderTargetView = mDisplay->GetRenderTargetView();
		
		context->ClearRenderTargetView(renderTargetView, clearColor);


	}
	void Narco_Deferred_Legacy::drawScreen()
	{
	}
}
