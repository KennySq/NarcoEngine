#include"../inc/GUI_Material.h"

namespace NARCO
{
	GUI_Material::GUI_Material(ID3D11Device* device)
		: mDevice(device), mTextureSlots(1024)
	{
		for (uint i = 0; i < 1024; i++)
		{
			mTextureSlots[i].resize(128);
		}
	}
	void GUI_Material::Start()
	{
		
	}
	void GUI_Material::Update()
	{
		ImGui::Text("Material Editor");

		if (mGameObject != nullptr)
		{
			uint materialCount = mMaterials.size();
			
			for (uint i = 0; i < materialCount; i++)
			{
				Material* material = mMaterials[i];
				SharedPipelineResource<ID3D11ShaderResourceView>& shaderResources = material->GetShaderResources();
				auto& resourceMap = shaderResources.Map;
				uint resourceCount = resourceMap.size();
				uint resourceIndex = 0;
				
				mTextureSlots[i].resize(resourceCount);

				for (auto& r : resourceMap)
				{
					GUI_FileSlot* fileSlot = mTextureSlots[i][resourceIndex];

					ImGui::PushID(resourceIndex);
					fileSlot->Update();

					ImGui_Texture* texture = fileSlot->GetImage();
					if (texture != nullptr)
					{
						ID3D11ShaderResourceView* srv = texture->GetSRV();
						if (srv != nullptr)
						{
							r.second->Resource = texture->Migrate();
						}
					}

					ImGui::PopID();

					resourceIndex++;
				}
			}
		}
	}
	void GUI_Material::Draw()
	{
	}
	void GUI_Material::End()
	{
		uint materialSize = mMaterials.size();

		for (uint i = 0; i < materialSize; i++)
		{
			uint slotSize = mTextureSlots[i].size();

			for (uint j = 0; j < slotSize; j++)
			{
				mTextureSlots[i][j]->End();
			}
		}

	}
	void GUI_Material::onChangeGameObject()
	{
		uint materialSize = mMaterials.size();
		for (uint i = 0; i < materialSize; i++)
		{
			Material* material = mMaterials[i];
			auto ShaderResources = material->GetShaderResources();
			uint slotSize = material->GetShaderResources().Map.size();
			auto itr = material->GetShaderResources().Map.begin();

			for (uint j = 0; j < slotSize; j++)
			{
				GUI_FileSlot* slot = mTextureSlots[i][j];
				if (slot == nullptr)
				{
					slot = new GUI_FileSlot(ASSET_IMAGE, mDevice, "");
					mTextureSlots[i][j] = slot;
				}
				
				long long nameHash = ShaderResources.FindName(itr->second);
				nameHash += mGameObject->GetInstanceID();

				if (nameHash != -1)
				{
					slot->SetSlotName(std::to_string(nameHash));
				}

				itr++;
			}
		}

	}
}