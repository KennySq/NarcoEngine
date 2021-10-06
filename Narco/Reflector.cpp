#include"inc/Reflector.h"

namespace NARCO
{
	Reflector::Reflector(ID3D11Device* device, ID3DBlob* blob)
	{
		HRESULT result = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), 
			__uuidof(ID3D11ShaderReflection),
			reinterpret_cast<void**>(mReflection.GetAddressOf()));

		if (result != S_OK)
		{
			Debug::Log("failed to reflect shader.");
			return;
		}

		D3D11_SHADER_DESC shaderDesc{};

		result = mReflection->GetDesc(&shaderDesc);

		if (result != S_OK)
		{
			Debug::Log("failed to get shader descriptor");
			return;
		}

		mConstantBufferCount = shaderDesc.ConstantBuffers;
		mBoundResourceCount = shaderDesc.BoundResources;
		
	}
	Reflector::~Reflector()
	{
	}
	ComPtr<ID3D11InputLayout> Reflector::ReflectInputLayout(ID3D11Device* device, ID3DBlob* blob)
	{
		HRESULT result;
		D3D11_SHADER_DESC shaderDesc{};

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;

		result = mReflection->GetDesc(&shaderDesc);
		if (result != S_OK)
		{
			Debug::Log("failed to get shader descriptor.");
		}

		for (unsigned int i = 0; i < shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC parameterDesc{};

			mReflection->GetInputParameterDesc(i, &parameterDesc);

			D3D11_INPUT_ELEMENT_DESC elementDesc{};

			elementDesc.SemanticName = parameterDesc.SemanticName;
			elementDesc.SemanticIndex = parameterDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			if (parameterDesc.Mask == 1)
			{
				if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32_UINT;
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32_SINT;
				}
			}
			else if (parameterDesc.Mask <= 3)
			{
				if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				}
			}
			else if (parameterDesc.Mask <= 7)
			{
				if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				}
			}
			else if (parameterDesc.Mask <= 15)
			{
				if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
				else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				{
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				}
			}

			inputElements.emplace_back(elementDesc);

		}

		ComPtr<ID3D11InputLayout> il;
		
		result = device->CreateInputLayout(inputElements.data(), inputElements.size(), blob->GetBufferPointer(), blob->GetBufferSize(), il.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create input layout");
			return nullptr;
		}

		return il;
	}
	MaterialConstantProperty* Reflector::ReflectConstantBuffer(ID3D11Device* device, uint i)
	{
		HRESULT result;
		D3D11_SHADER_DESC shaderDesc{};
		ID3D11ShaderReflection* reflect = mReflection.Get();

		result = reflect->GetDesc(&shaderDesc);
		if (result != S_OK)
		{
			Debug::Log(std::string(shaderDesc.Creator) + "failed to get descriptor");
			return nullptr;
		}

		D3D11_SHADER_BUFFER_DESC bufDesc{};

		auto reflectCBuffer = reflect->GetConstantBufferByIndex(i);
		if (reflectCBuffer == nullptr)
		{
			Debug::Log(std::to_string(i) + " failed to get cbuffer.");
			return nullptr;
		}

		result = reflectCBuffer->GetDesc(&bufDesc);
		if (result != S_OK)
		{
			Debug::Log("failed to get cbuffer descriptor");
			return nullptr;
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
			return nullptr;
		}

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

		return mcp;
	}
	MaterialProperty* Reflector::ReflectTexture(ID3D11Device* device, uint i)
	{
		HRESULT result;

		D3D11_SHADER_DESC shaderDesc{};
		ID3D11ShaderReflection* reflect = mReflection.Get();
		
		result = reflect->GetDesc(&shaderDesc);
		if (result != S_OK)
		{
			Debug::Log("failed to get shader descriptor");
			return nullptr;
		}

		uint boundResources = shaderDesc.BoundResources;

		D3D11_SHADER_INPUT_BIND_DESC shaderInputDesc{};

		ePropertyType type = PROPERTY_UNKNOWN;
		ePropertyDimension dimension = DIMENSION_UNKNOWN;
		determineChannel(shaderInputDesc.uFlags, dimension);

		result = reflect->GetResourceBindingDesc(i, &shaderInputDesc);
		if (result != S_OK)
		{
			Debug::Log("failed to get resource binding descriptor");
			return nullptr;
		}
			
		if (shaderInputDesc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE)
		{
			if (shaderInputDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
			{
				type = ePropertyType::PROPERTY_TEXTURE2D;
			}
			else if (shaderInputDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE3D)
			{
				type = ePropertyType::PROPERTY_TEXTURE3D;
			}

		}

		else if (shaderInputDesc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED)
		{
			type = ePropertyType::PROPERTY_STRUCTURED;
		}

		else if (shaderInputDesc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TBUFFER)
		{
			type = ePropertyType::PROPERTY_BUFFER;
		}


		//-------------------------------------------------------------------------

		const char* name = shaderInputDesc.Name;
		long long hash = MakeHash(name);
		MP* mp = nullptr;
		if (type != PROPERTY_UNKNOWN && dimension != DIMENSION_UNKNOWN)
		{
			mp = new MaterialProperty(name, nullptr, type, dimension);
		}

		return mp;
	}
	MaterialUnorderProperty* Reflector::ReflectUnorderedAccess(ID3D11Device* device, uint i)
	{

		return nullptr;
	}
	MaterialOutputProperty* Reflector::ReflectRenderTarget(ID3D11Device* device, uint i)
	{
		return nullptr;
	}
	void Reflector::AddSRV(MP* mp)
	{
		long long hash = MakeHash(mp->Name);

		mTextures.insert_or_assign(hash, mp);

		mRawTextures.emplace_back(mp->Register.Get());

	}
	void Reflector::AddUAV(MUP* mup)
	{
		long long hash = MakeHash(mup->Name);

		auto result = mUnorderedAcceses.find(hash);

		if (result != mUnorderedAcceses.end())
		{
			return;
		}

		mUnorderedAcceses.insert_or_assign(hash, mup);

		mRawUnorderAccesses.emplace_back(mup->Register.Get());

	}
	void Reflector::AddBuffer(MCP* mcp)
	{
		long long hash = MakeHash(mcp->Name);

		mConstBuffers.insert_or_assign(hash, mcp);

		mRawConstBuffers.emplace_back(mcp->Buffer.Get());
	}

	void Reflector::determineChannel(uint mask, ePropertyDimension& dimension)
	{
		if (mask == 1)
		{
			dimension = ePropertyDimension::DIMENSION_FLOAT;
		}
		else if (mask >= 3)
		{
			dimension = ePropertyDimension::DIMENSION_FLOAT2;
		}
		else if (mask >= 8)
		{
			dimension = ePropertyDimension::DIMENSION_FLOAT3;
		}
		else if (mask >= 12)
		{
			dimension = ePropertyDimension::DIMENSION_FLOAT4;
		}

		return;
	}
}