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

	Light* Scene::AddLight(Light* light)
	{
		eLightMode mode = light->GetMode();
		eLightType type = light->GetType();

		switch (mode)
		{
		case eLightMode::LIGHT_REALTIME:


			break;
		}

		switch (type)
		{
		case eLightType::LIGHT_DIRECTIONAL:
			
			break;
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
	bool Scene::GenerateLightBuffer()
	{
		if (mLightBuffer != nullptr)
		{
			mLightBuffer.ReleaseAndGetAddressOf();
		}

		D3D11_BUFFER_DESC bufferDesc{};

		bufferDesc.ByteWidth = sizeof(mRealtimeLights.size());
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		
		HRESULT result = mDevice->CreateBuffer(&bufferDesc, nullptr, mLightBuffer.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to generate real-time light buffer.");
			return false;
		}

		return true;
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