#include "inc/Narco_Core.h"

using namespace NARCO;

namespace NARCO
{
	Narco_Core::Narco_Core(HWND windowHandle, HINSTANCE handleInst)
		: mWindowHandle(windowHandle), mHandleInstance(handleInst), mHardware(new D3DHardware()),
		mDisplay(new D3DDisplay(windowHandle, mHardware->GetDevice(), NARCO_INIT_APP_WIDTH, NARCO_INIT_APP_HEIGHT))
	{
		ID3D11Device* device = mHardware->GetDevice();
		ID3D11DeviceContext* context = mHardware->GetImmediateContext();



		unsigned int width = NARCO_INIT_APP_WIDTH;
		unsigned int height = NARCO_INIT_APP_HEIGHT;



		mGBuffer = new GBuffer(device, width, height);

		Shader* uberShader = new Shader("C:/Users/odess/Desktop/Projects/NarcoEngine/Narco/built-in/hlsl/Deferred_DefaultUber_0.hlsl", SHADER_VERTEX | SHADER_PIXEL);
		uberShader->Compile(device);
		Material* uberMaterial = new Material();

		// 테스트 코드 영역입니다.
		MeshLoader loader(device);

		loader.SetPath("C:/Users/odess/Desktop/Projects/NarcoEngine/Narco/x64/Debug/resources/shiba/shiba.fbx");
		loader.Load();

		Mesh* mesh_shiba = loader.ConvertMesh();

		mSelectedScene = new Scene("Sample Scene");

		AssetManager* assetManager = new AssetManager("C:/Users/odess/Desktop/Projects/NarcoEngine/Narco/x64/Debug/resources/app/assets");
	//	assetManager->AddSystemAsset("C:/Users/odess/Desktop/Projects/NarcoEngine/Narco/x64/Debug/resources/textures/system/icon_page.png");

		mMainCanvas = new GUI_Canvas(mWindowHandle, mHandleInstance, device, context);

		mMainCanvas->AddFrame(new GUI_Frame("File Slot", 300,300, ImGuiWindowFlags_NoResize));
		mMainCanvas->AddFrame(new GUI_Frame("Color Picker", 400,300, ImGuiWindowFlags_NoResize));
		mMainCanvas->AddFrame(new GUI_Frame("Asset Browser", 1100, 400, ImGuiWindowFlags_NoResize));
		mMainCanvas->AddFrame(new GUI_Frame("Shader Editor", 600, 800, ImGuiFocusedFlags_None));

		auto frame = mMainCanvas->GetFrame(0);
		auto frame2 = mMainCanvas->GetFrame(1);
		auto frame3 = mMainCanvas->GetFrame(2);
		auto frame4 = mMainCanvas->GetFrame(3);

		frame->AddGUI("FileSlot_01", new GUI_FileSlot());
		frame2->AddGUI("ColorPicker_01", new GUI_ColorPicker());
		frame3->AddGUI("AssetBrowser_01", new GUI_AssetManager(assetManager, device));
		frame4->AddGUI("ShaderEditor_01", new GUI_Material(uberShader));
		
	}
	void Narco_Core::Init()
	{
		mSelectedScene->start();

		mMainCanvas->Start();

	}
	void Narco_Core::Update(float delta)
	{
		static ID3D11DeviceContext* context = mHardware->GetImmediateContext();
		static ID3D11RenderTargetView* const * rtv = mGBuffer->GetRenderTargets();
		static ID3D11RenderTargetView* backBuffer[] = { mDisplay->GetRenderTargetView() };
		static ID3D11DepthStencilView* dsv = mGBuffer->GetDepthStencil();
		static D3D11_VIEWPORT viewports[] = { mDisplay->GetMainViewport() };

		static GUI_Canvas* canvas = mMainCanvas;
		static GUI_Frame* frame_ColorPicker = canvas->GetFrame(1);
		static GUI_ColorPicker* gui_ColorPicker = static_cast<GUI_ColorPicker*>(frame_ColorPicker->GetGUI("ColorPicker_01"));
		
		clearScreen(gui_ColorPicker->GetColor4());

		//context->OMSetRenderTargets(mGBuffer->GetBufferCount(), rtv, dsv);
		context->OMSetRenderTargets(1, backBuffer , dsv);
		context->RSSetViewports(1, viewports);
		


		mSelectedScene->update(delta);

		mMainCanvas->Update();

	}
	void Narco_Core::Render(float delta)
	{
		mSelectedScene->render(delta);
		mMainCanvas->Draw();


		mDisplay->Present();
		


	}
	void Narco_Core::Release()
	{
		mSelectedScene->release();

		mMainCanvas->End();

		delete mHardware;
		delete mDisplay;
	}
	void Narco_Core::clearScreen(const float* clearColor)
	{
		static ID3D11DeviceContext* context = mHardware->GetImmediateContext();
		static ID3D11RenderTargetView* renderTargetView = mDisplay->GetRenderTargetView();
		
		context->ClearRenderTargetView(renderTargetView, clearColor);


	}
}
