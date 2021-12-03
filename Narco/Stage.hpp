#pragma once
#include"inc/Stage.h"

#include<atlbase.h>
#include<d3dcompiler.h>
#include<d3d11shader.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace NARCO
{
	// update here. 2021/11/30 5:48 PM
	template<typename _ShaderTy>
	inline Stage<_ShaderTy>::Stage(const Stage& stage)
		: mPath(stage.mPath), mFileName(stage.mFileName), mShader(stage.mShader), mReflection(stage.mReflection), mByteCodes(stage.mByteCodes)
	{

	}

	template<>
	inline Stage<ID3D11VertexShader>::Stage(const char* shaderPath, ID3D11ClassLinkage* classLinkage)
		: mPath(shaderPath), mStageFlag(STAGE_VERTEX)
	{
		HRESULT result;

		result = compile("vert", "vs_5_0");

		ID3D11VertexShader** shader = reinterpret_cast<ID3D11VertexShader**>(mShader.GetAddressOf());
		result = mDevice->CreateVertexShader(mByteCodes->GetBufferPointer(), mByteCodes->GetBufferSize(), classLinkage, shader);

		if (result != S_OK)
		{
			Debug::Log("failed to create shader");
			return;
		}
	}
	template<>
	inline Stage<ID3D11GeometryShader>::Stage(const char* shaderPath, ID3D11ClassLinkage* classLinkage)
		: mPath(shaderPath), mStageFlag(STAGE_GEOMETRY)
	{
		HRESULT result;
		compile("geom", "gs_5_0");

		ID3D11GeometryShader** shader = reinterpret_cast<ID3D11GeometryShader**>(mShader.GetAddressOf());
		result = mDevice->CreateGeometryShader(mByteCodes->GetBufferPointer(), mByteCodes->GetBufferSize(), classLinkage, shader);

		if (result != S_OK)
		{
			Debug::Log("failed to create shader");
			return;
		}
	}
	template<>
	inline Stage<ID3D11DomainShader>::Stage(const char* shaderPath, ID3D11ClassLinkage* classLinkage)
		: mPath(shaderPath), mStageFlag(STAGE_DOMAIN)
	{
		HRESULT result;
		result = compile("doma", "ds_5_0");

		ID3D11DomainShader** shader = reinterpret_cast<ID3D11DomainShader**>(mShader.GetAddressOf());
		result = mDevice->CreateDomainShader(mByteCodes->GetBufferPointer(), mByteCodes->GetBufferSize(), classLinkage, shader);

		if (result != S_OK)
		{
			Debug::Log("failed to create shader");
			return;
		}
	}
	template<>
	inline Stage<ID3D11HullShader>::Stage(const char* shaderPath, ID3D11ClassLinkage* classLinkage)
		: mPath(shaderPath), mStageFlag(STAGE_HULL)
	{
		HRESULT result;
		result = compile("hull", "hs_5_0");

		ID3D11HullShader** shader = reinterpret_cast<ID3D11HullShader**>(mShader.GetAddressOf());
		result = mDevice->CreateHullShader(mByteCodes->GetBufferPointer(), mByteCodes->GetBufferSize(), classLinkage, shader);

		if (result != S_OK)
		{
			Debug::Log("failed to create shader");
			return;
		}
	}
	template<>
	inline Stage<ID3D11PixelShader>::Stage(const char* shaderPath, ID3D11ClassLinkage* classLinkage)
		: mPath(shaderPath), mStageFlag(STAGE_PIXEL)
	{
		HRESULT result;
		result = compile("frag", "ps_5_0");

		ID3D11PixelShader** shader = reinterpret_cast<ID3D11PixelShader**>(mShader.GetAddressOf());
		result = mDevice->CreatePixelShader(mByteCodes->GetBufferPointer(), mByteCodes->GetBufferSize(), classLinkage, shader);

		if (result != S_OK)
		{
			Debug::Log("failed to create shader");
			return;
		}
	}
	template<typename _ShaderTy>
	inline Stage<_ShaderTy>::~Stage()
	{
		uint bufferCount = mBuffers.size();
		for (uint i = 0; i < bufferCount; i++)
		{
			delete mBuffers[i];
		}

		uint shaderResourceCount = mShaderResources.size();
		for (uint i = 0; i < shaderResourceCount; i++)
		{
			delete mShaderResources[i];
		}

		uint samplerCount = mSamplerStates.size();
		for (uint i = 0; i < samplerCount; i++)
		{
			delete mSamplerStates[i];
		}

		if (mClassInstances != nullptr)
		{
			delete mClassInstances;
		}

	}
	template<typename _ShaderTy>
	inline HRESULT Stage<_ShaderTy>::compile(const char* entry, const char* model)
	{
		USES_CONVERSION;

		HRESULT result;

		DWORD compileFlag = 0;

		ID3DBlob* errBlob = nullptr;

#ifdef _DEBUG
		compileFlag |= D3DCOMPILE_DEBUG;
#endif

		const char* path = mPath.c_str();

		result = D3DCompileFromFile(A2W(path), nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, entry, model,
			compileFlag, 0, mByteCodes.GetAddressOf(), &errBlob);

		if (result != S_OK)
		{
#ifdef _DEBUG
			std::cout << reinterpret_cast<const char*>(errBlob->GetBufferPointer()) << '\n';
#endif

			if (result == HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND))
			{
				Debug::Log(mPath + " => invalid directory.");
				return result;
			}

			if (result == E_INVALIDARG)
			{
				Debug::Throw("invalid argument.");
				return result;
			}
		}

		result = D3DReflect(mByteCodes->GetBufferPointer(),
			mByteCodes->GetBufferSize(), __uuidof(ID3D11ShaderReflection),
			reinterpret_cast<void**>(mReflection.GetAddressOf()));

		if (result != S_OK)
		{
			Debug::Log("failed to reflect.");
			return result;
		}

		return result;
	}

	template<typename _ShaderTy>
	HRESULT Stage<_ShaderTy>::Reflect(SharedPipelineResource<ID3D11ShaderResourceView>* sharedResources)
	{
		D3D11_SHADER_DESC shaderDesc{};

		HRESULT result = mReflection->GetDesc(&shaderDesc);
		if (result != S_OK)
		{
			Debug::Log("failed to get shader descriptor.");
			return result;
		}

		for (uint i = 0; i < shaderDesc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC bindDesc{};
			result = mReflection->GetResourceBindingDesc(i, &bindDesc);
			if (result != S_OK)
			{
				Debug::Log("faield to get bound resource.");
				return result;
			}

			D3D_RESOURCE_RETURN_TYPE returnType = bindDesc.ReturnType;
			D3D_SRV_DIMENSION dimension = bindDesc.Dimension;
			D3D_SHADER_INPUT_TYPE type = bindDesc.Type;

			long long hash = MakeHash(bindDesc.Name);
			auto resource = sharedResources->Find(bindDesc.Name);
			if (resource != nullptr)
			{
				resource->StageFlags |= mStageFlag;

				mShaderResources.emplace_back(resource);

				continue;
			}

			if (type == D3D_SIT_TEXTURE)
			{
				resource = new SharedResource<ID3D11ShaderResourceView>();

				uint pixelSize = 0;

				resource->Name = bindDesc.Name;
				resource->StageFlags |= mStageFlag;

				sharedResources->Add(resource);

				mShaderResources.emplace_back(resource);

			}
		}

		return result;
	}

	template<typename _ShaderTy>
	HRESULT Stage<_ShaderTy>::Reflect(SharedPipelineResource<ID3D11SamplerState>* sharedResources)
	{
		D3D11_SHADER_DESC shaderDesc{};

		HRESULT result = mReflection->GetDesc(&shaderDesc);
		if (result != S_OK)
		{
			Debug::Log("failed to get shader descriptor.");
			return result;
		}
		return result;
	}

	template<typename _ShaderTy>
	inline HRESULT Stage<_ShaderTy>::Reflect(SharedPipelineResource<ID3D11ClassInstance>* sharedResources, ID3D11ClassLinkage* linkage, const std::string& variableName, const std::vector<std::string>& classInstances)
	{
		uint interfaceCount = mReflection->GetNumInterfaceSlots();

		if (interfaceCount < 0)
		{
			return E_FAIL;
		}

		ID3D11ShaderReflectionVariable* variable = mReflection->GetVariableByName(variableName.c_str());
		SharedResource<ID3D11ClassInstance>* sr = new SharedResource<ID3D11ClassInstance>();

		sr->Name = variableName;
		sr->StageFlags |= mStageFlag;
		sr->Instances = static_cast<ID3D11ClassInstance**>(malloc(sizeof(ID3D11ClassInstance*) * interfaceCount));
		sr->mInstanceCount = interfaceCount;

		mClassInstances = sr;

		for (uint i = 0; i < interfaceCount; i++)
		{
			uint interfaceSlot = variable->GetInterfaceSlot(i);

			linkage->GetClassInstance(classInstances[i].c_str(), i, &mClassInstances->Instances[i]);//±¸ÇöºÎ )

		}

		return S_OK;
	}

	template<typename _ShaderTy>
	HRESULT Stage<_ShaderTy>::Reflect(SharedPipelineResource<ID3D11Buffer>* sharedResources)
	{
		D3D11_SHADER_DESC shaderDesc{};

		HRESULT result = mReflection->GetDesc(&shaderDesc);
		if (result != S_OK)
		{
			Debug::Log("failed to get shader descriptor.");
			return result;
		}

		auto& variableOffsets = sharedResources->VariableOffsets;

		uint resourceCount = shaderDesc.BoundResources;
		D3D11_SHADER_INPUT_BIND_DESC resourceDesc{};

		for (uint i = 0; i < resourceCount; i++)
		{
			result = mReflection->GetResourceBindingDesc(i, &resourceDesc);

			if (resourceDesc.Type == D3D_SIT_CBUFFER)
			{
				// check same resource on the stage
				if (sharedResources->Find(resourceDesc.Name) != nullptr)
				{
					long long hash = MakeHash(resourceDesc.Name);
					auto resource = sharedResources->Find(resourceDesc.Name);

					resource->StageFlags |= mStageFlag;

					mBuffers.emplace_back(resource);

					continue;
				}

				SharedResource<ID3D11Buffer>* buffer = new SharedResource<ID3D11Buffer>();
				D3D11_BUFFER_DESC cbufferDesc{};
				D3D11_SHADER_BUFFER_DESC cbufferShaderDesc{};
				ID3D11ShaderReflectionConstantBuffer* cbufferShader = mReflection->GetConstantBufferByName(resourceDesc.Name);

				buffer->Name = resourceDesc.Name;

				result = cbufferShader->GetDesc(&cbufferShaderDesc);

				uint elementCount = cbufferShaderDesc.Variables;

				cbufferDesc.ByteWidth = cbufferShaderDesc.Size;
				cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;

				if (cbufferShaderDesc.Type == D3D_CT_CBUFFER)
				{
					for (uint i = 0; i < elementCount; i++)
					{
						auto variable = cbufferShader->GetVariableByIndex(i);
						D3D11_SHADER_VARIABLE_DESC variableDesc{};

						result = variable->GetDesc(&variableDesc);
						long long hash = MakeHash(variableDesc.Name);

						variableOffsets.insert_or_assign(hash, variableDesc.StartOffset);
					}

					result = mDevice->CreateBuffer(&cbufferDesc, nullptr, buffer->Resource.GetAddressOf());
					if (result != S_OK)
					{
						Debug::Log("failed to create constant buffer->");
						continue;
					}
					buffer->StageFlags |= mStageFlag;

					sharedResources->Add(buffer);
					mBuffers.emplace_back(buffer);

				}

			}
		}


		return result;
	}
}
