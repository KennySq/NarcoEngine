#include"inc/Shiba.h"

using namespace NARCO;

Shiba::Shiba()
	: Prefab("Shiba")
{
}

Shiba::~Shiba()
{
}

void Shiba::awake()
{
	Shader* uberShader = new Shader("built-in/hlsl/Deferred_DefaultUber_0.hlsl", SHADER_VERTEX | SHADER_PIXEL);
	uberShader->Compile(mDevice);
	Material* uberMaterial = new Material(uberShader, mDevice, mContext);


	MeshLoader loader(mDevice);

	loader.SetPath("C:/Users/odess/Desktop/Projects/NarcoEngine/Narco/x64/Debug/resources/shiba/shiba.fbx");
	loader.Load();

	Mesh* mesh_shiba = loader.ConvertMesh();

	Renderer* shibaRenderer = AddComponent<Renderer>();
	shibaRenderer->SetMesh(mesh_shiba);
	shibaRenderer->SetMaterial(uberMaterial);


	GameObject::awake();
}

void Shiba::start()
{

	mTransform->Rotate(0, -90, 0);

	GameObject::start();
}

void Shiba::update(float delta)
{

	GameObject::update(delta);

}

void Shiba::render(float delta)
{

	GameObject::render(delta);
}

void Shiba::release()
{

	GameObject::release();
}
