#pragma once
#include<Common.h>
#include<Material.h>

#include"IGUI.h"

namespace NARCO
{
	enum eMaterialInputType
	{
		INPUT_SCALAR,
		INPUT_COLOR3,
		INPUT_COLOR4,
		INPUT_TEXTURE2D,
		INPUT_TEXTURE3D,
	};

	class GUI_Material : public IGUI
	{
		PUBLIC_API NARCO_API GUI_Material(Material* material);
		PUBLIC_API NARCO_API virtual ~GUI_Material();
		PUBLIC_API virtual NARCO_API void Start() override;
		PUBLIC_API virtual NARCO_API void Update() override;
		PUBLIC_API virtual NARCO_API void Draw() override;
		PUBLIC_API virtual NARCO_API void End() override;
		
		PRIVATE_PROPERTY Material* mMaterial = nullptr;

	};
}
