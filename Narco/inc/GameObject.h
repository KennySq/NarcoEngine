#pragma once
#include"Common.h"
#include"Component.h"
#include"Trasnform.h"

namespace NARCO
{
	class Scene;
	typedef unsigned long long InstanceID;

	class GameObject
	{
		friend Scene;

	public:
		GameObject();
		GameObject(const char* name);
		GameObject(const GameObject& gameObject);
		GameObject(const GameObject&& gameObject);

		~GameObject();

		template<class _Comp>
		_Comp* GetComponent()
		{
			type_info typeInfo = typeid(_Comp);
			size_t compHash = typeInfo.hash_code();
			auto result = mComponents.find(compHash);

			if (mComponents.end() != result)
			{
				return result->second;
			}

			ExceptionError(E_INVALIDARG, "This gameobject doesn't have such component.");
			throw std::invalid_argument("This gameobject doesn't have such component.");
		}

		template<class _Comp>
		void AddComponent()
		{
			if (this == nullptr)
			{
				ExceptionError(E_INVALIDARG, "This gameObject is not valid.");
				throw std::invalid_argument("This gameObject is not valid.");
			}

			const type_info& typeInfo = typeid(_Comp);
			size_t compHash = typeInfo.hash_code();

			

			auto result = mComponents.find(compHash);

			if (mComponents.end() != result)
			{
				ExceptionError(E_INVALIDARG, "This component is already exist.");
				throw std::invalid_argument("This component is already exist.");
			}

			Component* newComp = new _Comp(); // preventing non-inherited object creation.

			newComp->mRoot = this;
			mComponents.insert_or_assign(compHash, newComp);

			return;
		}

	private:
		static long long mInstanceIDCount;

		void start();
		void update(float delta);
		void render(float delta);
		void release();

		std::string mName;
		std::map<ComponentID, Component*> mComponents;
		InstanceID mInstanceID;

		Scene* mScene;

		GameObject* mRoot;
		GameObject* mParent;
		std::vector<GameObject*> mChilds;
	};
}
