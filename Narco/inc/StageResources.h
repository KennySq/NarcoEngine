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
			: StageFlags(0), Resource(nullptr) {}
		SharedResource<_Ty>(const SharedResource<_Ty>& other)
			: Name(other.Name), StageFlags(other.StageFlags)
		{
			
		}
		~SharedResource<_Ty>()
		{
			if (Resource != nullptr)
			{
				Resource.ReleaseAndGetAddressOf();
			}
		}

		ComPtr<_Ty> Resource;
		std::string Name;

		uint StageFlags;
	};

	template<>
	struct SharedResource<ID3D11ClassInstance>
	{
		SharedResource()
			: StageFlags(0), Instances(nullptr) {}
		SharedResource(const SharedResource<ID3D11ClassInstance>& other)
			: Name(other.Name), StageFlags(other.StageFlags)
		{

		}
		
		~SharedResource()
		{
			if (Instances != nullptr)
			{
				delete Instances;
			}
		}

		ID3D11ClassInstance** Instances;
		uint mInstanceCount;

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
		long long FindName(SharedResource<_Ty>* sr);

		void Add(SharedResource<_Ty>* resource);

		ComPtr<ID3D11DeviceChild> Shader;
	};

}

#include"../StageResources.hpp"
