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

		MeshLoader loader(mDevice);

		Shader* uber = new Shader("built-in/hlsl/Deferred_DefaultUber_0.hlsl", SHADER_VERTEX | SHADER_PIXEL);
		uber->Compile(mDevice);
		Material* uberMat = new Material(uber, mDevice, mContext);

		loader.SetPath("C:/Users/odess/Desktop/Projects/NarcoEngine/Narco/x64/Debug/resources/ice_dragon/ice_dragon.fbx");
		loader.Load();

		Mesh* mesh = loader.ConvertMesh();

		renderer->SetMesh(mesh);
		renderer->SetMaterial(uberMat);


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