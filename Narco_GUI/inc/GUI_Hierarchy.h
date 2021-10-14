#pragma once
#include"IGUI.h"
#include"GUI_GameObject.h"



namespace NARCO
{
	class Scene;
	class GameObject;
	class GUI_Hierarchy : public IGUI
	{
	public:
		NARCO_API GUI_Hierarchy(Scene* scene, GUI_GameObject* inspector);
		NARCO_API ~GUI_Hierarchy();

	private:
		virtual NARCO_API void Start() override;
		virtual NARCO_API void Update() override;
		virtual NARCO_API void Draw() override;
		virtual NARCO_API void End() override;

		Scene* mScene;

		std::vector<GameObject*> mGameObjects;

		GUI_GameObject* mInspector;

		uint mSelectedItem;
		uint mIterateItem;

	};
}
