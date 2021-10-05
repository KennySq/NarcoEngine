#include"inc/Reflector.h"

namespace NARCO
{
	Reflector::Reflector(ID3D11Device* device, ID3D11ShaderReflection* reflection)
		: mReflection(reflection)
	{



	}
	Reflector::~Reflector()
	{
	}
	void Reflector::reflectConstantBuffer(ID3D11Device* device)
	{
		HRESULT result;
		D3D11_SHADER_DESC shaderDesc{};
		ID3D11ShaderReflection* reflect = mReflection;

		result = reflect->GetDesc(&shaderDesc);
		if (result != S_OK)
		{
			Debug::Log(std::string(shaderDesc.Creator) + "failed to get descriptor");
			return;
		}

		for (uint i = 0; i < shaderDesc.ConstantBuffers; i++)
		{
			D3D11_SHADER_BUFFER_DESC bufDesc{};

			auto reflectCBuffer = reflect->GetConstantBufferByIndex(i);
			if (reflectCBuffer == nullptr)
			{
				Debug::Log(std::to_string(i) + " failed to get cbuffer.");
				continue;
			}

			result = reflectCBuffer->GetDesc(&bufDesc);
			if (result != S_OK)
			{
				Debug::Log("failed to get cbuffer descriptor");
				continue;
			}

			ComPtr<ID3D11Buffer> buffer;
			D3D11_BUFFER_DESC propBufferDesc{};

			propBufferDesc.ByteWidth = bufDesc.Size;
			propBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			uint cbufferVariables = bufDesc.Variables;
			uint cbufferSize = bufDesc.Size;

			std::vector<const char*> variableNames;

			result = device->CreateBuffer(&propBufferDesc, nullptr, buffer.GetAddressOf());
			if (result != S_OK)
			{
				Debug::Log("failed to create cbuffer.\nsize : " + std::to_string(cbufferSize));
				continue;
			}
			
			// reflect register
			// create buffer.
			// provide interface for connect property to material

			for (uint j = 0; j < cbufferVariables; j++)
			{
				D3D11_SHADER_VARIABLE_DESC varDesc{};
				auto variable = reflectCBuffer->GetVariableByIndex(j);
				if (variable == nullptr)
				{
					Debug::Log(std::to_string(j) + ", failed to get cbuffer variable");
					continue;
				}

				result = variable->GetDesc(&varDesc);
				if (result != S_OK)
				{
					Debug::Log("failed to get variable descriptor");
					continue;
				}

				variableNames.push_back(varDesc.Name);
			}

			const char* name = bufDesc.Name;
			

			MCP* mcp = new MCP(name, variableNames, buffer, cbufferSize);

			mConstBuffers.insert_or_assign(MakeHash(name), mcp);
		}

	}
	void Reflector::reflectTexture(ID3D11Device* device)
	{
		HRESULT result;

		D3D11_SHADER_DESC shaderDesc{};
		ID3D11ShaderReflection* reflect = mReflection;
		
		result = reflect->GetDesc(&shaderDesc);
		if (result != S_OK)
		{
			Debug::Log("failed to get shader descriptor");
			return;
		}

		uint boundResources = shaderDesc.BoundResources;
		
		D3D11_SHADER_INPUT_BIND_DESC shaderInputDesc{};

		for (uint i = 0; i < boundResources; i++)
		{
			ePropertyType type;
			ePropertyDimension dimension;

			result = reflect->GetResourceBindingDesc(i, &shaderInputDesc);
			if (result != S_OK)
			{
				Debug::Log("failed to get resource binding descriptor");
				continue;
			}
			
			if (shaderInputDesc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE)
			{

				if (shaderInputDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
				{
					type = ePropertyType::PROPERTY_TEXTURE2D;
					shaderInputDesc.
				}
			}

		}



	}
	void Reflector::reflectUnorderedAccess(ID3D11Device* device)
	{
	}
	void Reflector::reflectRenderTarget(ID3D11Device* device)
	{
	}
	void Reflector::determineChannel(uint mask, ePropertyDimension& dimension)
	{
		if (mask == 1)
		{
			dimension = ePropertyDimension::PROPERTY_SCALAR;
		}

		else if(mask )

	}
}