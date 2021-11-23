#include "inc/Narco_Core.h"

#include"inc/Dragon.h"
#include"inc/Skater.h"

#include"inc/Camera.h"

#include"inc/Material.h"


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
		mSelectedScene = new Scene("Sample Scene", context, this);

		Material* mat = new Material("built-in/hlsl/Deferred_DefaultUber_0.hlsl", STAGE_VERTEX | STAGE_PIXEL);
		mSelectedScene->AddMaterial(mat);

		//Shader* uberShader = new Shader("built-in/hlsl/Deferred_DefaultUber_0.hlsl", SHADER_VERTEX | SHADER_PIXEL);
		//uberShader->Compile(device);
		//mSelectedScene->AddShader(uberShader);

		//Shader* customShader = new Shader("built-in/hlsl/Deferred_CustomUber_0.hlsl", SHADER_VERTEX | SHADER_PIXEL);
		//customShader->Compile(device);
		//mSelectedScene->AddShader(customShader);

		MeshLoader shibaLoader(device);
		MeshLoader dragonLoader(device);
		MeshLoader skaterLoader(device);

		shibaLoader.SetPath("x64/Debug/resources/shiba/shiba.fbx");
		shibaLoader.Load();

		dragonLoader.SetPath("x64/Debug/resources/dragon-high.fbx");
		//dragonLoader.Load();

		//skaterLoader.SetPath("x64/Debug/resources/skater/model/skater.fbx");
		//skaterLoader.Load();
		Mesh* shibaMesh = shibaLoader.ConvertMesh();
		mSelectedScene->AddMesh(shibaMesh);


		//mSelectedScene->AddMesh(dragonLoader.ConvertMesh());
		//mSelectedScene->AddMesh(skaterLoader.ConvertMesh());




	//	mSelectedScene->AddMesh()

		GameObject* shiba = mSelectedScene->AddGameObject(new Shiba());
		GameObject* dragon = mSelectedScene->AddGameObject(new Dragon());
		GameObject* skater = mSelectedScene->AddGameObject(new Skater());

		GameObject* mainCamera = mSelectedScene->AddGameObject(new GameObject("Main Camera"));

		Camera* mainCam = mainCamera->AddComponent<Camera>();
		mainCam->SetDisplay(mDisplay);
		mainCamera->SetTag("Main Camera");


		mVoxelOctree = new SVO(device, context, shibaMesh, mainCam);
		mVoxelOctree->Load();

		mMainCanvas = new GUI_Canvas(mWindowHandle, mHandleInstance, device, context);

		//mMainCanvas->AddFrame(new GUI_Frame("File Slot", 300, 300, ImGuiWindowFlags_NoResize));
		mMainCanvas->AddFrame(new GUI_Frame("Color Picker", 400, 300, ImGuiWindowFlags_NoResize));
		//mMainCanvas->AddFrame(new GUI_Frame("Asset Browser", 1100, 400, ImGuiWindowFlags_NoResize));
		//mMainCanvas->AddFrame(new GUI_Frame("Inspector", 400, 800, ImGuiFocusedFlags_None));
		//mMainCanvas->AddFrame(new GUI_Frame("Material", 400, 800, ImGuiFocusedFlags_None));
		//mMainCanvas->AddFrame(new GUI_Frame("Hierarchy", 400, 800, ImGuiFocusedFlags_None));
	}
	void Narco_Deferred_Legacy::PreInit()
	{
		ID3D11Device* device = D3DHardware::GetDevice();

		mSelectedScene->awake();

		//auto frame = mMainCanvas->GetFrame(0);
		auto frame2 = mMainCanvas->GetFrame(0);
		//auto frame3 = mMainCanvas->GetFrame(2);
		//auto frame4 = mMainCanvas->GetFrame(3);
		//auto frame5 = mMainCanvas->GetFrame(4);
		//auto frame6 = mMainCanvas->GetFrame(5);

		AssetManager* assetManager = new AssetManager("C:/Users/odess/Desktop/Projects/NarcoEngine/Narco/x64/Debug/resources/app/assets");

		GameObject* shiba = mSelectedScene->GetGameObject(0);


		GameObject* dragon = mSelectedScene->GetGameObject(1);


		GameObject* skater = mSelectedScene->GetGameObject(2);
		//frame->AddGUI("FileSlot_01", new GUI_FileSlot(ASSET_IMAGE, device));
		frame2->AddGUI("ColorPicker_01", new GUI_ColorPicker());
		//frame3->AddGUI("AssetBrowser_01", new GUI_AssetManager(assetManager, device));
		//frame4->AddGUI("Inspector_01", new GUI_GameObject(shiba));

		//GUI_GameObject* inspector = static_cast<GUI_GameObject*>(frame4->GetGUI("Inspector_01"));
		//frame6->AddGUI("Hierarchy_01", new GUI_Hierarchy(mSelectedScene, inspector));
	}
	void Narco_Deferred_Legacy::Init()
	{
		mSelectedScene->start();

		mMainCanvas->Start();

	}
	void Narco_Deferred_Legacy::Update(float delta)
	{
		static ID3D11DeviceContext* context = mHardware->GetImmediateContext();
		static ID3D11RenderTargetView* const* rtv = mGBuffer->GetRenderTargets();
		static ID3D11RenderTargetView* backBuffer[] = { mDisplay->GetRenderTargetView() };
		static ID3D11DepthStencilView* dsv = mGBuffer->GetDepthStencil();
		static D3D11_VIEWPORT viewports[] = { mDisplay->GetMainViewport() };
		static ID3D11ShaderResourceView* nullSrv[6] = { nullptr };

		static GUI_Canvas* canvas = mMainCanvas;
		static GUI_Frame* frame_ColorPicker = canvas->GetFrame(0);
		static GUI_ColorPicker* gui_ColorPicker = static_cast<GUI_ColorPicker*>(frame_ColorPicker->GetGUI("ColorPicker_01"));

		clearScreen(gui_ColorPicker->GetColor4());
		mGBuffer->ClearBuffer(context, gui_ColorPicker->GetColor4());

		context->RSSetViewports(1, viewports);
		mGBuffer->Bound(context);

		mSelectedScene->update(delta);

		mGBuffer->Unbound(context);
		context->PSSetShaderResources(0, 1, &nullSrv[0]);



		mGBuffer->DrawScreen(context, backBuffer[0], mVoxelOctree->GetDebugTexture());


		context->PSSetShaderResources(0, 1, &nullSrv[0]);

		mVoxelOctree->Compute();

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
		context->PSSetShaderResources(0, 1, &nullSrv[0]);



		mDisplay->Present();



	}
	void Narco_Deferred_Legacy::Release()
	{
		mSelectedScene->release();

		mMainCanvas->End();

		delete mHardware;
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
