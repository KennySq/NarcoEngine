#include"../inc/Backplane.h"
#include"../inc/Renderer.h"
#include"../inc/Mesh.h"

namespace NARCO
{
    BackPlane::BackPlane()
        : Prefab("BackPlane")
    {
    }
    BackPlane::~BackPlane()
    {
    }
    void BackPlane::awake()
    {
        const Scene* scene = GetScene();
        Renderer* renderer = AddComponent<Renderer>();
        Mesh* planeMesh = scene->GetMesh("Backplane.fbx");
        Material* defaultUber = scene->GetMaterial("Deferred_DefaultUber_0.hlsl");

        renderer->SetMesh(planeMesh);
        renderer->AddMaterial(defaultUber->MakeInstance());

        GameObject::awake();
    }
    void BackPlane::start()
    {
        mTransform->SetScale(5, 5, 5);
        mTransform->SetRotation(90, 180, -90);

        //  mTransform->SetRotation(0, 0, 0);
        mTransform->Translate(10, -2, 0);
        GameObject::start();
    }
    void BackPlane::update(float delta)
    {
 //       mTransform->Rotate(0.0f, 0.0f, 0.0f);

        GameObject::update(delta);
    }
    void BackPlane::render(float delta)
    {
        GameObject::render(delta);
    }
    void BackPlane::release()
    {
        GameObject::release();
    }
}