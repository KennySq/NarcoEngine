#pragma once
#include"inc\Stage.h"

#include<atlbase.h>
#include<d3dcompiler.h>
#include<d3d11shader.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace NARCO
{
	template<>
	inline Stage<ID3D11VertexShader>::Stage(const char* shaderPath, uint stageFlag)
		: mPath(shaderPath), mStageFlags(stageFlag)
	{
		compile("vert", "vs_5_0");
		
	}

	template<>
	inline Stage<ID3D11GeometryShader>::Stage(const char* shaderPath, uint stageFlag)
		: mPath(shaderPath), mStageFlags(stageFlag)
	{
		compile("geom", "gs_5_0");
	}

	template<>
	inline Stage<ID3D11DomainShader>::Stage(const char* shaderPath, uint stageFlag)
		: mPath(shaderPath), mStageFlags(stageFlag)
	{
		compile("doma", "ds_5_0");
	}

	template<>
	inline Stage<ID3D11HullShader>::Stage(const char* shaderPath, uint stageFlag)
		: mPath(shaderPath), mStageFlags(stageFlag)
	{
		compile("hull", "hs_5_0");
	}

	template<>
	inline Stage<ID3D11PixelShader>::Stage(const char* shaderPath, uint stageFlag)
		: mPath(shaderPath), mStageFlags(stageFlag)
	{
		compile("frag", "ps_5_0");
	}

	template<typename _ShaderTy>
	inline Stage<_ShaderTy>::~Stage()
	{
	}

	template<typename _ShaderTy>
	inline HRESULT Stage<_ShaderTy>::compile(const char* entry, const char* model)
	{
		USES_CONVERSION;

		HRESULT result;
		
		DWORD compileFlag = 0;

		ID3DBlob* blob = nullptr, *errBlob = nullptr;

#ifdef _DEBUG
		compileFlag |= D3DCOMPILE_DEBUG;
#endif

		result = D3DCompileFromFile(A2W(mPath.c_str()), nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, entry, model,
			compileFlag, 0, &blob, &errBlob);
		
		if (result != S_OK)
		{
			if (result == HRESULT_FROM_WIN32(D3D11_ERROR_FILE_NOT_FOUND))
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

		result = reflect(blob);

		if (result != S_OK)
		{
			return result;
		}
		return result;
	}

	template<typename _ShaderTy>
	inline HRESULT Stage<_ShaderTy>::reflect(ID3DBlob* blob)
	{
		HRESULT result;
		
		D3D11_SHADER_DESC shaderDesc{};

		result = D3DReflect(blob->GetBufferPointer(),
			blob->GetBufferSize(), __uuidof(ID3D11ShaderReflection),
			 reinterpret_cast<void**>(mReflection.GetAddressOf()));

		if (result != S_OK)
		{
			Debug::Log("failed to reflect.");
			return result;
		}

		result = mReflection->GetDesc(&shaderDesc);
		if (result != S_OK)
		{
			Debug::Log("failed to get shader descriptor.");
			return result;
		}

		uint resourceCount = shaderDesc.BoundResources;

		D3D11_SHADER_INPUT_BIND_DESC resourceDesc{};

		for (uint i = 0; i < resourceCount; i++)
		{
			result = mReflection->GetResourceBindingDesc(i, &resourceDesc);

			if (resourceDesc.Type == D3D_SIT_CBUFFER)
			{
				D3D11_BUFFER_DESC cbufferDesc{};
				D3D11_SHADER_BUFFER_DESC cbufferShaderDesc{};
				ID3D11ShaderReflectionConstantBuffer* cbufferShader = mReflection->GetConstantBufferByName(resourceDesc.Name);
				
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
					
					//	variableDesc.
					}
				}

				//else if (cbufferShaderDesc.Type == D3D_CT_INTERFACE_POINTERS)
				//{
				//	for (uint i = 0; i < elementCount; i++)
				//	{
				//		auto variable = cbufferShader->GetVariableByIndex(i);
				//		D3D11_SHADER_VARIABLE_DESC variableDesc{};

				//		result = variable->GetInterfaceSlot(i);
				//		

				//	}
				//}
				
			
			}
		}


		return result;
	}
}
