#include"inc\Skater.h"
#include"inc\MeshLoader.h"
#include"inc\Renderer.h"

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
	Renderer* renderer = AddComponent<Renderer>();

	const Scene* scene = GetScene();
	
	Shader* shader = scene->GetShader("Deferred_DefaultUber_0.hlsl");
	Shader* custom = scene->GetShader("Deferred_CustomUber_0.hlsl");

	Material* baseMap = new Material(shader, mDevice, mContext);
	Material* board = new Material(custom, mDevice, mContext);
	
	Mesh* mesh = scene->GetMesh("skater.fbx");
	
	renderer->SetMesh(mesh);
	renderer->AddMaterial(baseMap);
	renderer->AddMaterial(board);

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
