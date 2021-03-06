#pragma once
#include"Common.h"
#include"Super.h"

namespace NARCO
{
	class GameObject;
	typedef unsigned long long ComponentID;

	
	class Component : public Super
	{
		friend GameObject;

	public:
		Component() {}
		Component(const Component& c) = delete;
		Component(const Component&& c) = delete;
		virtual ~Component() {}

		const std::string& GetName() const { return mName; }
		const GameObject* GetRoot() const { return mRoot; }
		ComponentID GetID() const { return mComponentID; }

	protected:
		virtual void awake() = 0;
		virtual void start() = 0;
		virtual void update(float delta) = 0;
		virtual void render(float delta) = 0;
		virtual void release() = 0;

		std::string mName;

		GameObject* mRoot;

		ComponentID mComponentID;
	};
}