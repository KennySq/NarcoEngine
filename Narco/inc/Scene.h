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
		Scene(const char* name, ID3D11DeviceContext* context);
		~Scene();

		GameObject* FindGameObjectWithTag(const char* tag) const;

		GameObject* AddGameObject(GameObject* gameObject);
		GameObject* AddGameObject(const char* name);
		GameObject* GetGameObject(InstanceID iid) const;

		ID3D11DeviceContext* GetContext() const { return mContext; }

	private:

		void start();
		void update(float delta);
		void render(float delta);
		void release();

		void sceneRender();

		std::string mName;
		SceneID mSceneID;

		std::map<InstanceID, GameObject*> mGameObjects;
		
		ID3D11DeviceContext* mContext;

		// GameObject List
		// Cached Materials
		// 
	};
}