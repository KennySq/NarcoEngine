#pragma once
#include"Stage.h"
#include"StageResources.h"

namespace NARCO
{
	class Material : public Super
	{
	public:
		Material(const char* shaderPath, uint stageFlags);
		~Material();

		const std::string& GetShaderPath() const { return mShaderPath; }
		const std::string& GetFileName() const { return mFileName; }

		Stage<ID3D11VertexShader>* GetVertex() const { return mVertexStage; }
		Stage<ID3D11GeometryShader>* GetGeometry() const { return mGeometryStage; }
		Stage<ID3D11DomainShader>* GetDomain() const { return mDomainStage; }
		Stage<ID3D11HullShader>* GetHull() const { return mHullStage; }
		Stage<ID3D11PixelShader>* GetPixel() const { return mPixelStage; }

		ID3D11InputLayout* GetInputLayout() const { return mInputLayout.Get(); }

	private:

		Stage<ID3D11VertexShader>* mVertexStage;
		Stage<ID3D11GeometryShader>* mGeometryStage;
		Stage<ID3D11DomainShader>* mDomainStage;
		Stage<ID3D11HullShader>* mHullStage;
		Stage<ID3D11PixelShader>* mPixelStage;

		ComPtr<ID3D11InputLayout> mInputLayout;

		SharedPipelineResource<ID3D11Buffer> mBuffers;
		SharedPipelineResource<ID3D11ShaderResourceView> mShaderResources;
		SharedPipelineResource<ID3D11UnorderedAccessView> mUnorderAccesses;

		std::string mShaderPath;
		std::string mFileName;
		uint mStageFlags;
	};
}
