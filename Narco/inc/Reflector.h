#pragma once
#include<Common.h>

namespace NARCO
{
	class Reflector
	{
	public:
		Reflector(ID3D11ShaderReflection* reflection);
		~Reflector();


	private:
		
		ID3D11ShaderReflection* mReflection;

		unsigned int mConstantBufferCount;
		unsigned int mTextureImageCount;
		unsigned int mTextureBufferCount;
		unsigned int mUnorderedCount;
		unsigned int mSamplerCount;


	};
}