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
		SharedResource<_Ty>()
			: StageFlags(0) {}
		SharedResource<_Ty>(const SharedResource<_Ty>& other)
			: Resource(other.Resource), Name(other.Name), StageFlags(other.StageFlags)
		{

		}

		ComPtr<_Ty> Resource;
		std::string Name;

		uint StageFlags;
	};

	template<typename _Ty>
	struct SharedPipelineResource
	{
		std::map<long long, uint> VariableOffsets;
		std::map<long long, SharedResource<_Ty>*> Map;
		std::vector<_Ty*> Raw;

		SharedResource<_Ty>* Find(const std::string& name);
		void Add(SharedResource<_Ty>* resource);

		ComPtr<ID3D11DeviceChild> Shader;
	};

}

#include"../StageResources.hpp"
