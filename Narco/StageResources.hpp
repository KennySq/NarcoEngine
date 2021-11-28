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
	inline long long SharedPipelineResource<_Ty>::FindName(SharedResource<_Ty>* sr)
	{
		for (auto i : Map)
		{
			if (i.second == sr)
			{
				return i.first;
			}
		}

		return -1;
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

		Raw.push_back(resource->Resource.Get());
		Map.insert_or_assign(hash, resource);
	}
}