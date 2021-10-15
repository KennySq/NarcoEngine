#pragma once
#include<Common.h>
#include"Super.h"
#include"StageResources.h"

namespace NARCO
{
	enum eStageType
	{
		STAGE_VERTEX = 1,
		STAGE_GEOMETRY = 2,
		STAGE_DOMAIN = 4,
		STAGE_HULL = 8,
		STAGE_PIXEL = 16,
	  //STAGE_COMPUTE?
	};

	template<typename _ShaderTy>
	class Stage : public Super
	{
	public:
		Stage(const char* shaderPath);
		~Stage();

		_ShaderTy* GetShader() const { return static_cast<_ShaderTy*>(mShader.Get()); }
		
		const std::string& GetPath() const { return mPath; }
		const std::string& GetFileName() const { return mFileName; }
		
		HRESULT Reflect(SharedPipelineResource<ID3D11Buffer>* sharedResources);
		HRESULT Reflect(SharedPipelineResource<ID3D11ShaderResourceView>* sharedResources);
		HRESULT Reflect(SharedPipelineResource<ID3D11SamplerState>* sharedResources);

		uint GetBufferCount() const { return mBuffers.size(); }
		uint GetShaderResourceCount() const { return mShaderResources.size(); }
		uint GetSamplerStateCount() const { return mSamplerStates.size(); }

		ID3D11Buffer* const* GetBufferPointer() const { return mBuffers.data(); }
		ID3D11ShaderResourceView* const* GetShaderResources() const { return mShaderResources.data(); }
		ID3D11SamplerState* const* GetSamplerStates() const { return mSamplerStates.data(); }

		ID3D11ShaderReflection* GetReflection() const { return mReflection.Get(); }
		ID3DBlob* GetByteCode() const { return mByteCodes.Get(); }

	private:
		HRESULT compile(const char* entry, const char* model);

		std::vector<ID3D11Buffer*> mBuffers;
		std::vector<ID3D11ShaderResourceView*> mShaderResources;
		std::vector<ID3D11SamplerState*> mSamplerStates;

		ComPtr<ID3D11DeviceChild> mShader = nullptr;
		ComPtr<ID3D11ShaderReflection> mReflection;

		std::string mPath;
		std::string mFileName;

		eStageType mStageFlag;

		ComPtr<ID3DBlob> mByteCodes;

	};
}

#include"../Stage.hpp"
