#pragma once
#include"Common.h"
#include"Component.h"
#include"Trasnform.h"
#include"Super.h"
namespace NARCO
{
	class Scene;
	typedef unsigned long long InstanceID;

	class GameObject : public Super
	{
		friend Scene;

	public:
		GameObject();
		GameObject(const char* name);
		GameObject(const GameObject& gameObject);
		GameObject(const GameObject&& gameObject);

		virtual ~GameObject();

		const Scene* GetScene() const { return mScene; }

		void SetTag(const char* tag) { mTag = tag; }
		const char* GetTag() const { return mTag.c_str(); }


		template<class _Comp>
		_Comp* GetComponent()
		{
			const type_info& typeInfo = typeid(_Comp);
			size_t compHash = typeInfo.hash_code();
			auto result = mComponents.find(compHash);

			if (mComponents.end() != result)
			{
				return static_cast<_Comp*>(result->second);
			}

			ExceptionError(E_INVALIDARG, "This gameobject doesn't have such component.");
			throw std::invalid_argument("This gameobject doesn't have such component.");
		}

		template<class _Comp>
		_Comp* AddComponent()
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

			return static_cast<_Comp*>(newComp);
		}

	protected:
		static long long mInstanceIDCount;

		virtual void awake();
		virtual void start();
		virtual void update(float delta);
		virtual void render(float delta);
		virtual void release();

		std::string mName;
		std::map<ComponentID, Component*> mComponents;
		InstanceID mInstanceID;

		Scene* mScene;

		GameObject* mRoot;
		GameObject* mParent;
		std::vector<GameObject*> mChilds;

		std::string mTag;
	};
}
