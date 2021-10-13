#include"../inc/GUI_Material.h"
#include<Asset.h>
namespace NARCO
{
	GUI_Material::GUI_Material(Material* material, ID3D11Device* device, uint materialIndex)
		: mMaterial(material), mDevice(device), mMaterialIndex(materialIndex)
	{
		
		
		
	}
	GUI_Material::~GUI_Material()
	{
	}
	NARCO_API void GUI_Material::Start()
	{
		static auto& constRegisters = mMaterial->GetBuffers();
		static auto& textureRegisters = mMaterial->GetTextures();
		static auto& unorders = mMaterial->GetUnorders();

		const Shader* shader = mMaterial->GetShader();
		Reflector* vertRef = shader->GetVertexReflect();
		
		
		static char buf[64] = "";
		
		D3D11_BUFFER_DESC bufDesc{};

		for(auto& i : constRegisters)
		{
			auto buffer = i.second->Buffer.Get();
			auto variables = i.second->VariableNames;
			buffer->GetDesc(&bufDesc);
			uint size = bufDesc.ByteWidth;

			// some data (various size)
			for (auto& j : variables)
			{
				ePropertyDimension dim = j.second.Dimension;
				uint size = j.second.Size;
				MaterialInput input;

				if (dim == DIMENSION_FLOAT4X4)
				{
					XMFLOAT4X4* mat = new XMFLOAT4X4();
					input.Data = mat;
					input.Size = size;
				}
				else if (dim == DIMENSION_FLOAT4)
				{
					XMFLOAT4* vec = new XMFLOAT4();
					input.Data = vec;
					input.Size = size;
				}

				mConstantVariables.insert_or_assign(j.first, input);
			}



		}

		bTextureLoadOpen = new bool[textureRegisters.size()];
		for (uint i = 0; i < textureRegisters.size(); i++)
		{
			mGUIFileSlots.emplace_back(GUI_FileSlot(ASSET_IMAGE, mDevice));
		}
	}
	NARCO_API void GUI_Material::Update()
	{
		static auto& constRegisters = mMaterial->GetBuffers();
		static auto& textureRegisters = mMaterial->GetTextures();
		static auto& unorders = mMaterial->GetUnorders();
		
		static char buf[64] = "";
		static D3D11_BUFFER_DESC bufferDesc{};
		

		//for (auto i : constRegisters)
		//{
		//	auto variables = i.second->VariableNames;
		//	auto buffer = i.second->Buffer.Get();
		//	uint size;
		//	//const MaterialInput& input = mConstantVariables[buffer];
		//	
		//	buffer->GetDesc(&bufferDesc);

		//	for (auto j : variables)
		//	{
		//		const MaterialInput& input = mConstantVariables[j.first];
		//		ePropertyDimension dim = j.second.Dimension;

		//		if (dim == DIMENSION_FLOAT4X4)
		//		{
		//			XMFLOAT4X4* mat = reinterpret_cast<XMFLOAT4X4*>(input.Data);

		//			std::string label1 = (j.first + std::string("_0"));
		//			std::string label2 = (j.first + std::string("_1"));
		//			std::string label3 = (j.first + std::string("_2"));
		//			std::string label4 = (j.first + std::string("_3"));

		//			bool changed0 = ImGui::InputFloat4(label1.c_str(), mat->m[0]);
		//			bool changed1 = ImGui::InputFloat4(label2.c_str(), mat->m[1]);
		//			bool changed2 = ImGui::InputFloat4(label3.c_str(), mat->m[2]);
		//			bool changed3 = ImGui::InputFloat4(label4.c_str(), mat->m[3]);

		//			if (changed0 || changed1 || changed2 || changed3)
		//			{
		//				mMaterial->UpdateConstant(mat, i.first);
		//			}


		//			ImGui::Separator();
		//		}

		//	}
		//		
		//}
		uint texIndex = 0;
		std::string set = std::string("set ") + std::to_string(mMaterialIndex);
		
		ImGui::PushID(set.c_str());

		for (auto t : textureRegisters)
		{
			ImGui::PushID(texIndex);
			if (ImGui::Button(t.second->Name))
			{
				bTextureLoadOpen[texIndex] = !bTextureLoadOpen[texIndex];
			}
			ImGui::NewLine();

			if (bTextureLoadOpen[texIndex] == true)
			{
				loadTexture(&mGUIFileSlots[texIndex], t.second);
			}

			texIndex++;

			ImGui::PopID();
		}

		ImGui::PopID();

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
	NARCO_API void GUI_Material::loadTexture(GUI_FileSlot* slot, MaterialProperty* mp)
	{
		
		slot->Update();

		ImGui_Texture* texture = slot->GetImage();
		if (texture != nullptr)
		{
			ID3D11ShaderResourceView* srv = texture->GetSRV();
			mp->SetSRV(srv);
		}

		

		return;
	}
}