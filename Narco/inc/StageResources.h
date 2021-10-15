#pragma once
#include<Common.h>

namespace NARCO
{
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
		bool Find(const std::string& name)
		{
			if (Map.find(MakeHash(name)) != Map.end())
			{
				return true;
			}

			return false;
		}

		void Add(const SharedResource<_Ty>& resource)
		{
			bool bExists = Find(resource.Name);

			if (bExists == true)
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