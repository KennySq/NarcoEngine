#include"inc/Scene.h"
#include"MeshLoader.h"
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
		mGameObjectList.emplace_back(gameObject);

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
		mGameObjectList.emplace_back(gameObject);

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
	Material* Scene::AddMaterial(Material* mat)
	{
		if (mat == nullptr)
		{
			Debug::Log("invalid argument");
			return nullptr;
		}

		long long hash = MakeHash(mat->GetFileName());

		mCacheMaterials.insert_or_assign(hash, mat);

		return mat;
	}
	Material* Scene::GetMaterial(const char* name) const
	{
		long long hash = MakeHash(name);

		auto result = mCacheMaterials.find(hash);

		if (result == mCacheMaterials.end())
		{
			Debug::Log(std::string("Material ") + name + " not found.");
			return nullptr;
		}

		return result->second;
	}

	Light* Scene::AddLight(Light* light)
	{
		if (light == nullptr)
		{
			Debug::Log("invalid argument.");
			return nullptr;
		}

		eLightType type = light->GetType();

		if (type == LIGHT_REALTIME)
		{
			mRealtimeLights.emplace_back(light);
		}
		else if (type == LIGHT_BAKE)
		{
			mBakeLights.emplace_back(light);
		}

		return light;
	}

	bool Scene::updateLightBuffer()
	{
		static LightHandler* lightHandler = LightHandler::GetInstance();

		std::vector<PointLight> pointLights;
		std::vector<DirectionalLight> directionalLights;

		uint lightCount = mRealtimeLights.size();

		for (uint i = 0; i < lightCount; i++)
		{
			eLightType type = mRealtimeLights[i]->GetType();
			if (type == eLightType::LIGHT_POINT)
			{
				Light* light = mRealtimeLights[i];
				pointLights.emplace_back(*reinterpret_cast<PointLight*>(light));
			}
			else if (type == eLightType::LIGHT_DIRECTIONAL)
			{
				Light* light = mRealtimeLights[i];
				directionalLights.emplace_back(*reinterpret_cast<DirectionalLight*>(light));
			}
		}



		return true;
	}
	void Scene::awake()
	{
		static LightHandler* lightHandler = LightHandler::GetInstance();

		lightHandler->OnChangeScene(this);

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