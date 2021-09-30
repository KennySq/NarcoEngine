#include"inc/GameObject.h"

namespace NARCO
{
	long long GameObject::mInstanceIDCount = 0;

	GameObject::GameObject()
		: mInstanceID(mInstanceIDCount++), mName(std::string("New GameObject ") + std::to_string(mInstanceID))
		, mTag("No Tag")
	{
		AddComponent<Transform>();
	}
	GameObject::GameObject(const char* name)
		: mInstanceID(mInstanceIDCount++), mName(name), mTag("No Tag")
	{
		AddComponent<Transform>();

	}
	GameObject::GameObject(const GameObject& gameObject)
		: mInstanceID(mInstanceIDCount++), mTag("No Tag")
	{


	}
	GameObject::GameObject(const GameObject&& gameObject)
		: mInstanceID(mInstanceIDCount++), mTag("No Tag")
	{
	}
	GameObject::~GameObject()
	{
	}
	void GameObject::start()
	{
		for (auto c : mComponents)
		{
			Component* comp = c.second;

			comp->start();
		}
	}

	void GameObject::update(float delta)
	{
		for (auto c : mComponents)
		{
			Component* comp = c.second;

			comp->update(delta);
		}
	}

	void GameObject::render(float delta)
	{
		for (auto c : mComponents)
		{
			Component* comp = c.second;

			comp->render(delta);
		}
	

	}

	void GameObject::release()
	{
		for (auto c : mComponents)
		{
			Component* comp = c.second;

			comp->release();
		}
	}
}