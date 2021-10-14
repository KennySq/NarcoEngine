#pragma once
#include<Common.h>
#include"Super.h"


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

	struct ConstantBufferVariable
	{
		std::string Name;
		uint StartOffset;



	};

	template<typename _ShaderTy>
	class Stage : public Super
	{
	public:
		Stage(const char* shaderPath, uint stageFlag);
		~Stage();


		_ShaderTy* GetShader() const { return static_cast<_ShaderTy*>(mShader.Get()); }
		
		const std::string& GetPath() const { return mPath; }
		const std::string& GetFileName() const { return mFileName; }


	private:

		HRESULT compile(const char* entry, const char* model);
		HRESULT reflect(ID3DBlob* blob);

		ComPtr<ID3D11DeviceChild> mShader = nullptr;
		
		std::vector<ID3D11Buffer*> mBuffers;
		std::vector<ID3D11ShaderResourceView*> mShaderResources;
		std::vector<ID3D11UnorderedAccessView*> mUnorderAccesses;
		std::vector<ID3D11SamplerState*> mSamplerStates;

		ComPtr<ID3D11ShaderReflection> mReflection;

		std::string mPath;
		std::string mFileName;

		uint mStageFlags;
	};
}

#include"../Stage.hpp"
