#pragma once
#include"Common.h"

#include"GameObject.h"

namespace NARCO
{
	class Narco_Deferred_Legacy;
	typedef unsigned long long SceneID;

	class Scene
	{
		friend Narco_Deferred_Legacy;

	public:
		Scene(const char* name);
		~Scene();

		void AddGameObject(GameObject* gameObject);
		GameObject* GetGameObject(InstanceID iid) const;

	private:

		void start();
		void update(float delta);
		void render(float delta);
		void release();

		void sceneRender();

		std::string mName;
		SceneID mSceneID;

		std::map<InstanceID, GameObject*> mGameObjects;
		
		// GameObject List
		// Cached Materials
		// 
	};
}