#include"../inc/GUI_GameObject.h"

namespace NARCO
{
	NARCO_API GUI_GameObject::GUI_GameObject(GameObject* gameObject)
		: mGameObject(gameObject)
	{
	}
	NARCO_API void GUI_GameObject::Start()
	{
		
		return;
	}
	NARCO_API void GUI_GameObject::Update()
	{
		const auto& components = mGameObject->GetComponents();

		for (auto c : components)
		{
			Component* comp = c.second;
			std::string name = comp->GetName();
			name = name.substr(name.find_last_of(':') + 1);

			ImGui::Text(name.c_str());
		}

		return;
	}
	NARCO_API void GUI_GameObject::Draw()
	{
		return;
	}
	NARCO_API void GUI_GameObject::End()
	{
		return;
	}
}