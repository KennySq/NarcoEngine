#pragma once
#include"Common.h"

#include"GameObject.h"
#include"Prefab.h"

namespace NARCO
{
	class Narco_Deferred_Legacy;
	typedef unsigned long long SceneID;

	class Scene
	{
		friend Narco_Deferred_Legacy;

	public:
		Scene(const char* name, ID3D11DeviceContext* context, Narco_Deferred_Legacy* rp);
		~Scene();

		GameObject* FindGameObjectWithTag(const char* tag) const;

		GameObject* AddGameObject(GameObject* gameObject);
		GameObject* AddGameObject(const char* name);
		GameObject* GetGameObject(InstanceID iid) const;

		ID3D11DeviceContext* GetContext() const { return mContext; }

		Narco_Deferred_Legacy* GetRP() const { return mRenderPipeline; }
	private:

		void awake();
		void start();
		void update(float delta);
		void render(float delta);
		void release();

		std::string mName;
		SceneID mSceneID;

		std::map<InstanceID, GameObject*> mGameObjects;
		
		ID3D11DeviceContext* mContext;
		Narco_Deferred_Legacy* mRenderPipeline;

		// GameObject List
		// Cached Materials
		// 
	};
}