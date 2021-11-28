#pragma once
#include"IGUI.h"
#include"ImGui_Texture.h"

#include"GUI_FileSlot.h"

#include<Asset.h>
#include<Material.h>
#include<Renderer.h>

namespace NARCO
{
	class GUI_Material : public IGUI
	{
	public:
		NARCO_API GUI_Material(ID3D11Device* device);

		NARCO_API virtual void Start() override;
		NARCO_API virtual void Update() override;
		NARCO_API virtual void Draw() override;
		NARCO_API virtual void End() override;


		NARCO_API void SetGameObject(GameObject* gameObject)
		{
			mGameObject = gameObject;
			Renderer* renderer = gameObject->GetComponent<Renderer>();
			if (renderer == nullptr)
			{
				mGameObject = nullptr;
				return;
			}

			Material** materials = renderer->GetMaterials();
			uint materialCount = renderer->GetMaterialCount();

			if (mMaterials.size() > 0)
			{
				return;
			}

			for (uint i = 0; i < materialCount; i++)
			{
				mMaterials.push_back(materials[i]);
			}

			onChangeGameObject();
		}

		NARCO_API GameObject* GetGameObject() const { return mGameObject; }

	private:
		NARCO_API void onChangeGameObject();

		ID3D11Device* mDevice;
		std::vector<Material*> mMaterials;
		std::vector<std::vector<GUI_FileSlot*>> mTextureSlots;

		GameObject* mGameObject;

	};
}