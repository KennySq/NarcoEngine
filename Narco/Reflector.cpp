#include"inc/Reflector.h"

namespace NARCO
{
	Reflector::Reflector(ID3D11ShaderReflection* reflection)
		: mReflection(reflection)
	{
		D3D11_SHADER_DESC shaderDesc{};

		reflection->GetDesc(&shaderDesc);



	}
	Reflector::~Reflector()
	{
	}
}