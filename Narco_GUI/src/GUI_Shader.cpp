#include"../inc/GUI_Shader.h"

namespace NARCO
{
	GUI_Material::GUI_Material(Material* material)
		: mMaterial(material)
	{
		
		
		
	}
	GUI_Material::~GUI_Material()
	{
	}
	NARCO_API void GUI_Material::Start()
	{
		return;
	}
	NARCO_API void GUI_Material::Update()
	{
		static auto registers = mMaterial->GetInputRegisters();
		static char buf[64] = "";

		for (unsigned int i = 0; i < registers.size(); i++)
		{
			if (registers[i].Type == D3D_SIT_UAV_RWTYPED)
			{
				ImGui::InputText(registers[i].Name, buf, 64, ImGuiInputTextFlags_CharsDecimal);
			}
		}
		return;
	}
	NARCO_API void GUI_Material::Draw()
	{
		return;
	}
	NARCO_API void GUI_Material::End()
	{
		return;
	}
}