#pragma once
#include"Common.h"

namespace NARCO
{
	class GameObject;
	typedef unsigned long long ComponentID;

	
	class Component
	{
		friend GameObject;

	public:
		Component(const char* name);
		Component(const Component& c) = delete;
		Component(const Component&& c) = delete;
		virtual ~Component() {}

	private:
		virtual void start() = 0;
		virtual void update(float delta) = 0;
		virtual void render(float delta) = 0;
		virtual void release() = 0;

		std::string mName;

		GameObject* mRoot;

		ComponentID mComponentID;
	};
}