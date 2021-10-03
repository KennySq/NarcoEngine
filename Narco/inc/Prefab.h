#pragma once
#include"GameObject.h"

namespace NARCO
{
	class Prefab : public GameObject
	{
		friend Scene;

	public:
		Prefab(const char* name);
		virtual ~Prefab();

	private:
		const char* mPrefabName;
		
	};
}