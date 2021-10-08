#pragma once
#include"IGUI.h"
#include<GameObject.h>
namespace NARCO
{
	class GUI_GameObject : public IGUI
	{
		PUBLIC_API NARCO_API GUI_GameObject(GameObject* gameObject);
		PUBLIC_API NARCO_API ~GUI_GameObject() {}
		// IGUI을(를) 통해 상속됨
		PUBLIC_API virtual NARCO_API void Start() override;
		PUBLIC_API virtual NARCO_API void Update() override;
		PUBLIC_API virtual NARCO_API void Draw() override;
		PUBLIC_API virtual NARCO_API void End() override;

		PRIVATE_PROPERTY GameObject* mGameObject;
	};
}