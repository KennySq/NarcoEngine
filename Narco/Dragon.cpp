#include"inc/Dragon.h"
#include"inc/MeshLoader.h"
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
		const Scene* scene = GetScene();

		Mesh* mesh = scene->GetMesh("dragon-high.fbx");

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