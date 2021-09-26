#include"inc/Scene.h"

namespace NARCO
{
	Scene::Scene(const char* name)
		: mName(name), mSceneID(MakeHash(mName))
	{
	}
	Scene::~Scene()
	{
	}
	void Scene::AddGameObject(GameObject* gameObject)
	{
		InstanceID id = gameObject->mInstanceID;

		if (mGameObjects.find(id) != mGameObjects.end())
		{
			ExceptionWarning(E_INVALIDARG, "This gameObject is already in the scene.");
			return;
		}

		gameObject->mScene = this;
		mGameObjects.insert_or_assign(id, gameObject);

		return;

	}
	GameObject* Scene::GetGameObject(InstanceID iid) const
	{
		if (mGameObjects.find(iid) != mGameObjects.end())
		{
			return mGameObjects.find(iid)->second;
		}

		return nullptr;
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