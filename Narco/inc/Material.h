#pragma once
#include"Stage.h"
#include"StageResources.h"

namespace NARCO
{
	class Material : public Super
	{
	public:
		Material(const char* shaderPath, uint stageFlags);
		Material(const Material& material);
		~Material();

		const std::string& GetShaderPath() const { return mShaderPath; }
		const std::string& GetFileName() const { return mFileName; }

		Stage<ID3D11VertexShader>* GetVertex() const { return mVertexStage; }
		Stage<ID3D11GeometryShader>* GetGeometry() const { return mGeometryStage; }
		Stage<ID3D11DomainShader>* GetDomain() const { return mDomainStage; }
		Stage<ID3D11HullShader>* GetHull() const { return mHullStage; }
		Stage<ID3D11PixelShader>* GetPixel() const { return mPixelStage; }

		ID3D11InputLayout* GetInputLayout() const { return mInputLayout.Get(); }

		SharedPipelineResource<ID3D11Buffer>& GetConstBuffers() { return mBuffers; }
		SharedPipelineResource<ID3D11ShaderResourceView>& GetShaderResources() { return mShaderResources; }
		SharedPipelineResource<ID3D11SamplerState>& GetSamplerStates() { return mSamplerStates; }

		Material* MakeInstance();

		void MapConstantBuffer(const char* cbufferName, const char* variableName, eResourceType type, void* data, uint size)
		{
			if (type == RESOURCE_CBUFFER)
			{
				auto result = mBuffers.Find(cbufferName);

				if (result == nullptr)
				{
					Debug::Log(std::string(cbufferName) + " not found.");
					return;
				}

				D3D11_MAPPED_SUBRESOURCE mappedSub{};
				HRESULT mapResult = mContext->Map(result->Resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSub);
				if (mapResult != S_OK)
				{
					Debug::Log("failed to map");
					return;
				}

				long long variableHash = MakeHash(variableName);
				const auto& variableOffsets = mBuffers.VariableOffsets;

				auto variableResult = variableOffsets.find(variableHash);

				if (variableResult == variableOffsets.end())
				{
					Debug::Log(std::string(variableName) + " not found.");
					return;
				}

				uint offset = variableResult->second;

				int* origin = reinterpret_cast<int*>(mappedSub.pData);

				void* ptr = origin + (offset / sizeof(int));

				memcpy(ptr, data, size);

				mContext->Unmap(result->Resource.Get(), 0);

			}
		}


	private:
		Stage<ID3D11VertexShader>* mVertexStage;
		Stage<ID3D11GeometryShader>* mGeometryStage;
		Stage<ID3D11DomainShader>* mDomainStage;
		Stage<ID3D11HullShader>* mHullStage;
		Stage<ID3D11PixelShader>* mPixelStage;

		ComPtr<ID3D11InputLayout> mInputLayout;

		SharedPipelineResource<ID3D11Buffer> mBuffers;
		SharedPipelineResource<ID3D11ShaderResourceView> mShaderResources;
		SharedPipelineResource<ID3D11SamplerState> mSamplerStates;

		std::string mShaderPath;
		std::string mFileName;
		uint mStageFlags;
	};
}
