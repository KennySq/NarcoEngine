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
			Debug::Log(gameObject->mName + " is already in the scene.");
			return nullptr;
		}

		gameObject->mScene = this;
		mGameObjects.insert_or_assign(id, gameObject);

		return gameObject;

	}
	GameObject* Scene::AddGameObject(const char* name)
	{
		
		InstanceID id = MakeHash(name);

		if (mGameObjects.find(id) != mGameObjects.end())
		{
			Debug::Log(std::string(name) + " is already in the scene.");
			return nullptr;
		}

		GameObject* gameObject = new GameObject(name);

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
	Shader* Scene::AddShader(Shader* shader)
	{
		if (shader == nullptr)
		{
			Debug::Log("invalid argument.");
			return nullptr;
		}

		std::string fileName = shader->GetFileName();

		long long hash = MakeHash(fileName);

		mCacheShaders.insert_or_assign(hash, shader);

		return shader;
	}
	Shader* Scene::GetShader(const char* name) const
	{
		long long hash = MakeHash(name);

		auto result = mCacheShaders.find(hash);

		if (result == mCacheShaders.end())
		{
			Debug::Log(std::string("shader ") + name + " not found.");
			return nullptr;
		}

		return result->second;
	}
	Mesh* Scene::AddMesh(Mesh* mesh)
	{
		if (mesh == nullptr)
		{
			Debug::Log("invalid argument");
			return nullptr;
		}

		long long hash = MakeHash(mesh->GetFileName());

		mCacheMeshes.insert_or_assign(hash, mesh);

		return mesh;

	}
	Mesh* Scene::GetMesh(const char* name) const
	{
		long long hash = MakeHash(name);

		auto result = mCacheMeshes.find(hash);

		if (result == mCacheMeshes.end())
		{
			Debug::Log(std::string("Mesh ") + name + " not found.");
			return nullptr;
		}

		return result->second;
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

}