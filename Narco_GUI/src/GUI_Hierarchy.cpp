#include"../inc/GUI_Hierarchy.h"
#include"../inc/GUI_Material.h"
#include<Scene.h>
#include<GameObject.h>

namespace NARCO
{
	NARCO_API GUI_Hierarchy::GUI_Hierarchy(Scene* scene, GUI_GameObject* inspector, GUI_Material* material)
		: mScene(scene), mInspector(inspector), mMaterial(material)
	{
		mIterateItem = 0;
		mSelectedItem = -1;

	}
	NARCO_API GUI_Hierarchy::~GUI_Hierarchy()
	{
	}
	NARCO_API void GUI_Hierarchy::Start()
	{
	}
	NARCO_API void GUI_Hierarchy::Update()
	{
		uint gameObjectCount = mScene->GetGameObjectCount();
		GameObject* const* gameObjects = mScene->GetGameObjects();

		for (uint i = 0; i < gameObjectCount; i++)
		{
			ImGui::PushID(i);
			
			const char* name = gameObjects[i]->GetName();
			uint iid = gameObjects[i]->GetInstanceID();

			bool bSelected = mSelectedItem == i;

			std::string label = std::to_string(iid);

			if (ImGui::Selectable(name, bSelected))
			{
				mSelectedItem = i;
			}

			if (bSelected)
			{
				ImGui::SetItemDefaultFocus();
				GameObject* gameObject = gameObjects[mSelectedItem];

				mInspector->SetGameObject(gameObject);
				mMaterial->SetGameObject(gameObject);

			}

			ImGui::PopID();
		}

	}
	NARCO_API void GUI_Hierarchy::Draw()
	{
	}
	NARCO_API void GUI_Hierarchy::End()
	{
	}
}