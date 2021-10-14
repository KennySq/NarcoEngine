#include"inc\Skater.h"
#include"inc\MeshLoader.h"

using namespace NARCO;

Skater::Skater()
	: Prefab("Skater")
{
}

Skater::~Skater()
{
}

void Skater::awake()
{
	const Scene* scene = GetScene();

	Mesh* mesh = scene->GetMesh("skater.fbx");
	
	GameObject::awake();
}

void Skater::start()
{
	
	GameObject::start();
}

void Skater::update(float delta)
{
	GameObject::update(delta);
}

void Skater::render(float delta)
{
	GameObject::render(delta);
}

void Skater::release()
{
	GameObject::release();
}
