#include"inc/Dragon.h"
#include"inc/Renderer.h"

namespace NARCO
{
	Dragon::Dragon()
		: Prefab("Dragon")
	{
	}
	Dragon::~Dragon()
	{
	}
	void Dragon::awake()
	{
		Renderer* renderer = AddComponent<Renderer>();
		const Scene* scene = GetScene();

		Shader* uber = scene->GetShader("Deferred_DefaultUber_0.hlsl");
		Material* uberMat = new Material(uber, mDevice, mContext);

		Mesh* mesh = scene->GetMesh("dragon-high.fbx");

		renderer->SetMesh(mesh);
		renderer->AddMaterial(uberMat);

		GameObject::awake();
	}
	void Dragon::start()
	{


		Transform* transform = GetComponent<Transform>();

		transform->SetPosition(-5, 0, 2);

		GameObject::start();
	}
	void Dragon::update(float delta)
	{

		GameObject::update(delta);
	}
	void Dragon::render(float delta)
	{
		GameObject::render(delta);
	}
	void Dragon::release()
	{
		GameObject::release();

	}
}