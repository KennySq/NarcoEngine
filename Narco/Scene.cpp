#include"inc/Scene.h"

namespace NARCO
{
	Scene::Scene(const char* name, ID3D11DeviceContext* context, Narco_Deferred_Legacy* rp)
		: mName(name), mSceneID(MakeHash(mName)), mContext(context), mRenderPipeline(rp)
	{
	}
	Scene::~Scene()
	{
	}
	GameObject* Scene::FindGameObjectWithTag(const char* tag) const
	{
		for (auto i : mGameObjects)
		{
			if (tag == i.second->mTag)
			{
				return i.second;
			}
		}
		return nullptr;
	}
	GameObject* Scene::AddGameObject(GameObject* gameObject)
	{
		InstanceID id = gameObject->mInstanceID;

		if (mGameObjects.find(id) != mGameObjects.end())
		{
			ExceptionWarning(E_INVALIDARG, "This gameObject is already in the scene.");
			return nullptr;
		}

		gameObject->mScene = this;
		mGameObjects.insert_or_assign(id, gameObject);

		return gameObject;

	}
	GameObject* Scene::AddGameObject(const char* name)
	{
		GameObject* gameObject = new GameObject(name);
		InstanceID id = gameObject->mInstanceID;

		if (mGameObjects.find(id) != mGameObjects.end())
		{
			ExceptionWarning(E_INVALIDARG, "This gameObject is already in the scene.");
			return nullptr;
		}

		gameObject->mScene = this;
		mGameObjects.insert_or_assign(id, gameObject);

		return gameObject;
	}
	GameObject* Scene::GetGameObject(InstanceID iid) const
	{
		if (mGameObjects.find(iid) != mGameObjects.end())
		{
			return mGameObjects.find(iid)->second;
		}

		return nullptr;
	}
	void Scene::awake()
	{
		for (auto i : mGameObjects)
		{
			Prefab* prefab = reinterpret_cast<Prefab*>(i.second);

			if (prefab != nullptr)
			{
				prefab->awake();
			}
			else
			{
				GameObject* inst = i.second;

				inst->awake();
			}


		}
	}
	void Scene::start()
	{
		for (auto i : mGameObjects)
		{
			GameObject* inst = i.second;

			inst->start();
		}
	}
	void Scene::update(float delta)
	{
		for (auto i : mGameObjects)
		{
			GameObject* inst = i.second;

			inst->update(delta);
			
		}

	}
	void Scene::render(float delta)
	{
		for (auto i : mGameObjects)
		{
			GameObject* inst = i.second;

			inst->render(delta);
		}
	}
	void Scene::release()
	{
		for (auto i : mGameObjects)
		{
			GameObject* inst = i.second;

			inst->release();
		}

	}
	void Scene::sceneRender()
	{

	}
}