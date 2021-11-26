#include"../inc/GUI_Material.h"

namespace NARCO
{
	GUI_Material::GUI_Material(ID3D11Device* device)
		: mDevice(device)
	{
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
				for (auto r : resourceMap)
				{
					GUI_FileSlot* fileSlot = mTextureSlots[i][resourceIndex];

					fileSlot->Update();

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
		if (materialSize > 0)
		{
			for (uint i = 0; i < materialSize; i++)
			{
				if (mTextureSlots.size() == 0)
				{
					break;
				}

				uint slotSize = mTextureSlots[i].size();

				for (uint j = 0; j < slotSize; j++)
				{
					if (mTextureSlots[i][j] != nullptr)
					{
						delete mTextureSlots[i][j];
					}
				}
				
			}

			mTextureSlots.resize(0);
		}

		for (uint i = 0; i < materialSize; i++)
		{
			Material* material = mMaterials[i];
			uint slotSize = material->GetShaderResources().Map.size();
			
			mTextureSlots.push_back(std::vector<GUI_FileSlot*>());

			for (uint j = 0; j < slotSize; j++)
			{
				mTextureSlots[i].push_back(new GUI_FileSlot(ASSET_IMAGE, mDevice));
			}
		}

	}
}