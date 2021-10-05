#pragma once
#include<Common.h>
#include"Material.h"
namespace NARCO
{
	class Reflector
	{
	public:
		Reflector(ID3D11Device* device, ID3D11ShaderReflection* reflection);
		~Reflector();

	private:
		
		void reflectConstantBuffer(ID3D11Device* device);
		void reflectTexture(ID3D11Device* device);
		void reflectUnorderedAccess(ID3D11Device* device);
		void reflectRenderTarget(ID3D11Device* device);

		void determineChannel(uint mask, ePropertyDimension& dimension);

		ID3D11ShaderReflection* mReflection;

		unsigned int mConstantBufferCount;
		unsigned int mTextureImageCount;
		unsigned int mTextureBufferCount;
		unsigned int mUnorderedCount;
		unsigned int mSamplerCount;

		std::map<long long, MCP> mConstBuffers;
		std::vector<ComPtr<ID3D11ShaderResourceView>> mShaderResources;
		std::vector<ComPtr<ID3D11RenderTargetView>> mRenderTargets;
		std::vector<ComPtr<ID3D11UnorderedAccessView>> mUnorderedAccess;
		
	};
}