#pragma once
#include<Common.h>
#include<Material.h>

#include"IGUI.h"
#include"ImGui_Texture.h"
#include"GUI_FileSlot.h"

namespace NARCO
{

	/*enum eMaterialInputType
	{
		INPUT_SCALAR,
		INPUT_COLOR2,
		INPUT_COLOR3,
		INPUT_COLOR4,
		INPUT_MATRIX4X4,
		INPUT_TEXTURE2D,
		INPUT_TEXTURE3D,
	};*/

	struct MaterialInput
	{
		MaterialInput() {}
		MaterialInput(void* data, uint size)
			: Data(data), Size(size) {}

		void* Data;
		uint Size;
	};


	class GUI_Material : public IGUI
	{
		PUBLIC_API NARCO_API GUI_Material(Material* material, ID3D11Device* device, uint materialIndex);
		PUBLIC_API NARCO_API virtual ~GUI_Material();
		PUBLIC_API virtual NARCO_API void Start() override;
		PUBLIC_API virtual NARCO_API void Update() override;
		PUBLIC_API virtual NARCO_API void Draw() override;
		PUBLIC_API virtual NARCO_API void End() override;
		
		PRIVATE_API NARCO_API void loadTexture(GUI_FileSlot* slot, MaterialProperty* mp);

		PRIVATE_PROPERTY ID3D11Device* mDevice;
		PRIVATE_PROPERTY Material* mMaterial = nullptr;
		PRIVATE_PROPERTY std::map<const char*, MaterialInput> mConstantVariables;
		PRIVATE_PROPERTY uint mMaterialIndex;

		PRIVATE_PROPERTY std::vector<GUI_FileSlot> mGUIFileSlots;

		bool* bTextureLoadOpen;

	};
}
