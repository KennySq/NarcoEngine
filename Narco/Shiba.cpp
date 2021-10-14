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
	const Scene* scene = GetScene();

	Mesh* mesh_shiba = scene->GetMesh("shiba.fbx");

	GameObject::awake();
}

void Shiba::start()
{
	mTransform->Translate(-500, 0, 0.125f);
	//mTransform->Rotate(0, 0, 0);

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
