#include"inc/Dragon.h"
#include"inc/Renderer.h"

namespace NARCO
{
	void Dragon::awake()
	{
		Renderer* renderer = AddComponent<Renderer>();

		MeshLoader loader(mDevice);

		Shader* uber = new Shader("built-in/hlsl/Deferred_DefaultUber_0.hlsl", SHADER_VERTEX | SHADER_PIXEL);
		Material* uberMat = new Material(uber, mDevice, mContext);

		loader.SetPath("resources/dragon-high.fbx");
		loader.Load();

		Mesh* mesh = loader.ConvertMesh();

		renderer->SetMesh(mesh);
		renderer->SetMaterial(uberMat);


		GameObject::awake();


	}
	void Dragon::start()
	{
		GameObject::start();
	}
	void Dragon::update(float delta)
	{
		GameObject::update(delta);
	}
	void Dragon::render(float delta)
	{
		GameObject::
	}
	void Dragon::release()
	{
	}
}