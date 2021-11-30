#include"inc/GameObject.h"

namespace NARCO
{
	long long GameObject::mInstanceIDCount = 0;

	GameObject::GameObject()
		: mInstanceID(mInstanceIDCount++), mName(std::string("New GameObject ") + std::to_string(mInstanceID))
		, mTag("No Tag")
	{
		mTransform = AddComponent<Transform>();
	}
	GameObject::GameObject(const char* name)
		: mInstanceID(mInstanceIDCount++), mName(name), mTag("No Tag")
	{
		mTransform = AddComponent<Transform>();

		

	}
	GameObject::GameObject(const GameObject& gameObject)
		: mInstanceID(mInstanceIDCount++), mTag("No Tag")
	{
	}
	GameObject::GameObject(const GameObject&& gameObject)
		: mInstanceID(mInstanceIDCount++), mTag("No Tag")
	{
		mTransform = gameObject.mTransform;
	}
	GameObject::~GameObject()
	{
	}
	void GameObject::awake()
	{
		auto transform = GetComponent<Transform>();

		transform->SetMatrix(XMMatrixIdentity());

		for (auto c : mComponents)
		{
			Component* comp = c.second;

			comp->awake();
		}
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
	GameObject::InstanceBuffer::InstanceBuffer(ID3D11Device* device, unsigned int size)
		: Size(size)
	{
		D3D11_BUFFER_DESC bufferDesc{};

		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		HRESULT result = device->CreateBuffer(&bufferDesc, nullptr, Buffer.GetAddressOf());
	
		if (result != S_OK)
		{
			Debug::Log("failed to create instance buffer");
			return;
		}
	}
}