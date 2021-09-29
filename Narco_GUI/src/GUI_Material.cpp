#include"../inc/GUI_Material.h"

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
		static auto registers = mMaterial->GetInputTextureRegisters();
		static char buf[64] = "";

		for (auto i = registers.begin(); i != registers.end(); ++i)
		{
			long long hash = MakeHash(i->second->Name);
			if (i->second->PropertyType == PROPERTY_BUFFER)
			{
				if (i->second->Dimension == PROPERTY_SCALAR)
				{
					static float v;
					ImGui::InputFloat(i->second->Name, &v);
					mMaterial->UpdateBuffer(&v, 1, hash);

				}

				else if (i->second->Dimension == PROPERTY_VECTOR2)
				{
					static float v[2];
					ImGui::InputFloat2(i->second->Name, v);
					mMaterial->UpdateBuffer(v, 2, hash);

				}

				else if (i->second->Dimension == PROPERTY_VECTOR3)
				{
					static float v[3];
					ImGui::InputFloat3(i->second->Name, v);
					mMaterial->UpdateBuffer(v, 3, hash);

				}
				else if (i->second->Dimension == PROPERTY_VECTOR4)
				{
					static float v[4];
					ImGui::InputFloat4(i->second->Name, v);

					mMaterial->UpdateBuffer(v, 4, hash);
				}

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