#include"inc/Shiba.h"

#include"inc/Renderer.h"

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
	const Scene* scene = GetScene();

	Mesh* mesh_shiba = scene->GetMesh("shiba.fbx");

	Material* uberMat = scene->GetMaterial("Deferred_DefaultUber_0.hlsl");

	Renderer* renderer = AddComponent<Renderer>();

	renderer->SetMesh(mesh_shiba);
	renderer->AddMaterial(uberMat);


	GameObject::awake();
}

void Shiba::start()
{
	mTransform->Translate(5.0f, .0f, 0.0f);

	GameObject::start();

	mTransform->Rotate(-90.0f, 0.0f, 0.0f);
}

void Shiba::update(float delta)
{

	GameObject::update(delta);
	mTransform->Rotate(0.0f, 0.25f, 0.0f);


}

void Shiba::render(float delta)
{

	GameObject::render(delta);
}

void Shiba::release()
{

	GameObject::release();
}
