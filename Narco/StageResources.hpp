#pragma once
#include"inc/StageResources.h"

namespace NARCO
{
	template<typename _Ty>
	SharedResource<_Ty>* SharedPipelineResource<_Ty>::Find(const std::string& name)
	{
		long long hash = MakeHash(name);
		auto result = Map.find(hash);
		if (result != Map.end())
		{
			return Map[hash];
		}

		return nullptr;
	}

	template<typename _Ty>
	void SharedPipelineResource<_Ty>::Add(SharedResource<_Ty>* resource)
	{
		auto result = Find(resource->Name);

		if (result != nullptr)
		{
			//Debug::Log(resource.Name + " is already exists.");
			return;
		}

		long long hash = MakeHash(resource->Name);

		Map.insert_or_assign(hash, resource);
	}
}