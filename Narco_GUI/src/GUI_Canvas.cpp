#include"GUI_Canvas.h"

namespace NARCO
{
	NARCO_API GUI_Canvas::GUI_Canvas(HWND winHandle, HINSTANCE handleInst, ID3D11Device* device, ID3D11DeviceContext* context)
		: mWinHandle(winHandle), mHandleInstance(handleInst), mDevice(device), mContext(context)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

	}
	NARCO_API GUI_Canvas::~GUI_Canvas()
	{
	}
	NARCO_API void GUI_Canvas::Start()
	{
		RECT rect{};
		GetClientRect(mWinHandle, &rect);

		int clientWidth = rect.right - rect.left;
		int clientHeight = rect.bottom - rect.top;

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = true;
		io.DisplaySize.x = clientWidth;
		io.DisplaySize.y = clientHeight;

		ImGui_ImplWin32_Init(mWinHandle);
		ImGui_ImplDX11_Init(mDevice, mContext);
		ImGui::StyleColorsDark();
		//ImGui::SetNextWindowSize(ImVec2(400, 300));
		//ImGui::SetNextWindowPos(ImVec2(50, 50));
		unsigned int frameCount = mGuiFrames.size();

		for (unsigned int i = 0; i < frameCount; i++)
		{
			mGuiFrames[i]->Start();
		}

	}
	
	NARCO_API void GUI_Canvas::Update()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		unsigned int frameCount = mGuiFrames.size();

		for (unsigned int i = 0; i < frameCount; i++)
		{


			mGuiFrames[i]->Update();
		}
	//	ImGui::EndFrame();

		return;
	}
	NARCO_API void GUI_Canvas::Draw()
	{


		ImGui::Render();
		ImDrawData* data = ImGui::GetDrawData();

		ImGui_ImplDX11_RenderDrawData(data);

		return;
	}
	NARCO_API void GUI_Canvas::End()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();

		unsigned int frameCount = mGuiFrames.size();

		for (unsigned int i = 0; i < frameCount; i++)
		{
			mGuiFrames[i]->End();
		}

	}
	NARCO_API void GUI_Canvas::AddFrame(GUI_Frame* frame)
	{
		mGuiFrames.emplace_back(frame);
	}
	NARCO_API GUI_Frame* GUI_Canvas::GetFrame(unsigned int i)
	{
		unsigned int count = mGuiFrames.size();
		if (i < 0 || i >= count)
		{
			Debug::Log(i + "Invalid index");
			return nullptr;
		}
		
		GUI_Frame* frame = reinterpret_cast<GUI_Frame*>(mGuiFrames[i]);

		return frame;
	}
}