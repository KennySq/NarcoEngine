#pragma once
#include<Common.h>

namespace NARCO
{
	enum eResourceType
	{
		RESOURCE_CBUFFER,
		RESOURCE_TEXTURE,
		RESOURCE_SAMPLER,
	};

	template<typename _Ty>
	struct SharedResource
	{
		SharedResource<_Ty>() {}
		SharedResource<_Ty>(const SharedResource<_Ty>& other)
			: Resource(other.Resource), Name(other.Name)
		{

		}

		ComPtr<_Ty> Resource;
		std::string Name;

		uint StageFlags;
	};

	template<typename _Ty>
	struct SharedPipelineResource
	{
		_Ty* Find(const std::string& name)
		{
			auto result = Map.find(MakeHash(name));
			if (result != Map.end())
			{
				return result.second.Get();
			}

			return nullptr;
		}

		void Add(const SharedResource<_Ty>& resource)
		{
			auto result = Find(resource.Name);

			if (result == nullptr)
			{
				//Debug::Log(resource.Name + " is already exists.");
				return;
			}

			long long hash = MakeHash(resource.Name);

			Map.insert_or_assign(hash, resource);
		}

		std::map<long long, SharedResource<_Ty>> Map;
		std::vector<_Ty*> Raw;

		ComPtr<ID3D11DeviceChild> Shader;
	};
}