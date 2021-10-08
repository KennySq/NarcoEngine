#pragma once
#include"Common.h"
#include"Component.h"
#include"Transform.h"
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
			if (this == nullptr)
			{
				Debug::Log( std::to_string(mInstanceID) + ", " + mName + "is not valid.");
			}

			const type_info& typeInfo = typeid(_Comp);
			size_t compHash = typeInfo.hash_code();
			auto result = mComponents.find(compHash);

			if (mComponents.end() != result)
			{
				return static_cast<_Comp*>(result->second);
			}
			
			Debug::Log(std::string(typeInfo.name()) + " not found.");
			

		}

		template<class _Comp>
		_Comp* AddComponent()
		{
			if (this == nullptr)
			{
				Debug::Log("this GameObject is not valid.");

			}

			const type_info& typeInfo = typeid(_Comp);
			size_t compHash = typeInfo.hash_code();

			auto result = mComponents.find(compHash);

			if (mComponents.end() != result)
			{
				Debug::Log(std::string(typeInfo.name()) + " is already exist.");
			}

			Component* newComp = new _Comp(); // preventing non-inherited object creation.

			newComp->mRoot = this;
			newComp->mComponentID = compHash;
			newComp->mName = typeid(_Comp).name();

			mComponents.insert_or_assign(compHash, newComp);

			return static_cast<_Comp*>(newComp);
		}

		const auto& GetComponents() const { return mComponents; }


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

		Transform* mTransform;

		std::vector<GameObject*> mChilds;

		std::string mTag;
	};
}
