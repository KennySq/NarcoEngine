#include"inc/Material.h"

namespace NARCO
{
	HRESULT Material::reflectVertex(ID3D11ShaderReflection* reflection)
	{
		HRESULT result;
		D3D11_SHADER_DESC shaderDesc{};
		std::vector<D3D11_SHADER_INPUT_BIND_DESC> inputRegisters;

		result = reflection->GetDesc(&shaderDesc);

		mShader->mVertexConstantBufferCount = shaderDesc.ConstantBuffers;
		mShader->mVertexBoundResourceCount = shaderDesc.BoundResources;

		if (result != S_OK)
		{
			Debug::Log("failed to get vertex shader descriptor");
			return result;
		}

		std::vector<D3D11_SIGNATURE_PARAMETER_DESC> inputParameters;
		std::vector<D3D11_SIGNATURE_PARAMETER_DESC> outputParameters;

		for (unsigned int i = 0; i < shaderDesc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC bindDesc{};
			D3D11_BUFFER_DESC bufferDesc{};
			result = reflection->GetResourceBindingDesc(i, &bindDesc);
			if (result != S_OK)
			{
				Debug::Log("failed to getting bound resource register");

				return E_FAIL;
			}

			if (bindDesc.Type == D3D_SIT_CBUFFER)
			{
				bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				ID3D11ShaderReflectionConstantBuffer* buffer = reflection->GetConstantBufferByIndex(bindDesc.BindPoint);

				D3D11_SHADER_BUFFER_DESC shaderBufferDesc{};
				std::vector<const char*> variableNames;

				result = buffer->GetDesc(&shaderBufferDesc);
				if (result != S_OK)
				{
					Debug::Log("failed to getting descriptor for constant buffer.");
					return result;
				}

				bufferDesc.ByteWidth = shaderBufferDesc.Size;

				for (unsigned int i = 0; i < shaderBufferDesc.Variables; i++)
				{
					ID3D11ShaderReflectionVariable* var = buffer->GetVariableByIndex(i);
					D3D11_SHADER_VARIABLE_DESC varDesc{};
					result = var->GetDesc(&varDesc);
					if (result != S_OK)
					{
						Debug::Log("failed to getting descriptor for constant buffer member.");
						return result;
					}

					variableNames.emplace_back(varDesc.Name);
				}

				ComPtr<ID3D11Buffer> constantBuffer;

				result = mDevice->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
				if (result != S_OK)
				{
					Debug::Log("invalid argument for creating constant buffer.");
					return result;
				}

				long long hash = MakeHash(shaderBufferDesc.Name);
				
				if (mInputConstantProperties.find(hash) != mInputConstantProperties.end())
				{
					continue;
				}
					
				MCP* constProperties = new MCP(shaderBufferDesc.Name,variableNames, constantBuffer, bufferDesc.ByteWidth, shaderBufferDesc.Variables);

				mInputConstantProperties.insert_or_assign(MakeHash(shaderBufferDesc.Name), constProperties);

			}
			inputRegisters.emplace_back(bindDesc);
		}
		return S_OK;
	}
	HRESULT Material::reflectPixel(ID3D11ShaderReflection* reflection)
	{
		HRESULT result;
		D3D11_SHADER_DESC shaderDesc{};
		std::vector<D3D11_SHADER_INPUT_BIND_DESC> inputRegisters;

		result = reflection->GetDesc(&shaderDesc);

		mShader->mPixelConstantBufferCount = shaderDesc.ConstantBuffers;
		mShader->mPixelBoundResourceCount = shaderDesc.BoundResources;

		for (unsigned int i = 0; i < shaderDesc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC bindDesc{};
			result = reflection->GetResourceBindingDesc(i, &bindDesc);
			if (result != S_OK)
			{
				Debug::Log("failed to getting bound resource register");
				return E_FAIL;
			}

			HRESULT result;
			D3D11_BUFFER_DESC bufferDesc{};
			D3D11_TEXTURE2D_DESC texture2dDesc{};
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			ePropertyDimension dimension;

			if (bindDesc.Type == D3D_SIT_TEXTURE) // t register
			{
				bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				texture2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

				if (bindDesc.Dimension == D3D_SRV_DIMENSION_BUFFER) // buffer
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
					srvDesc.Buffer.NumElements = 1;
					if (bindDesc.ReturnType == D3D11_RETURN_TYPE_FLOAT) // float
					{
						if (bindDesc.uFlags == 1)						// 1 Channel
						{
							bufferDesc.ByteWidth = sizeof(float);
							srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
							dimension = PROPERTY_SCALAR;
						}
						else if (bindDesc.uFlags <= 3)
						{
							bufferDesc.ByteWidth = sizeof(float) * 2;
							srvDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
							dimension = PROPERTY_VECTOR2;

						}
						else if (bindDesc.uFlags <= 8)
						{
							bufferDesc.ByteWidth = sizeof(float) * 3;
							srvDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
							dimension = PROPERTY_VECTOR3;

						}
						else if (bindDesc.uFlags <= 15)
						{
							bufferDesc.ByteWidth = sizeof(float) * 4;
							srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
							dimension = PROPERTY_VECTOR4;
						}
					}
					else if (bindDesc.ReturnType == D3D11_RETURN_TYPE_UINT) // float
					{
						if (bindDesc.uFlags == 1)						// 1 Channel
						{
							bufferDesc.ByteWidth = sizeof(unsigned int);
							srvDesc.Format = DXGI_FORMAT_R32_UINT;
							dimension = PROPERTY_SCALAR;
						}
						else if (bindDesc.uFlags <= 3)
						{
							bufferDesc.ByteWidth = sizeof(unsigned int) * 2;
							srvDesc.Format = DXGI_FORMAT_R32G32_UINT;
							dimension = PROPERTY_VECTOR2;


						}
						else if (bindDesc.uFlags <= 7)
						{
							bufferDesc.ByteWidth = sizeof(unsigned int) * 3;
							srvDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
							dimension = PROPERTY_VECTOR3;


						}
						else if (bindDesc.uFlags <= 15)
						{
							bufferDesc.ByteWidth = sizeof(unsigned int) * 4;
							srvDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
							dimension = PROPERTY_VECTOR4;
						}
					}

					else if (bindDesc.ReturnType == D3D11_RETURN_TYPE_SINT) // float
					{
						if (bindDesc.uFlags == 1)						// 1 Channel
						{
							bufferDesc.ByteWidth = sizeof(int);
							srvDesc.Format = DXGI_FORMAT_R32_SINT;
							dimension = PROPERTY_SCALAR;
						}
						else if (bindDesc.uFlags <= 3)
						{
							bufferDesc.ByteWidth = sizeof(int) * 2;
							srvDesc.Format = DXGI_FORMAT_R32G32_SINT;
							dimension = PROPERTY_VECTOR2;


						}
						else if (bindDesc.uFlags <= 7)
						{
							bufferDesc.ByteWidth = sizeof(int) * 3;
							srvDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
							dimension = PROPERTY_VECTOR3;


						}
						else if (bindDesc.uFlags <= 15)
						{
							bufferDesc.ByteWidth = sizeof(int) * 4;
							srvDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
							dimension = PROPERTY_VECTOR4;

						}
					}


					ComPtr<ID3D11Buffer> buffer;
					ComPtr<ID3D11ShaderResourceView> srv;
					result = mDevice->CreateBuffer(&bufferDesc, nullptr, buffer.GetAddressOf());
					if (result != S_OK)
					{
						Debug::Log("failed to create t-register buffer.");
						return result;
					}

					result = mDevice->CreateShaderResourceView(buffer.Get(), &srvDesc, srv.GetAddressOf());
					if (result != S_OK)
					{
						Debug::Log("failed to create t-register shader resource view.");
						return result;
					}

					mBuffers.emplace_back(buffer);
					mInputProperties.insert_or_assign(MakeHash(bindDesc.Name), new MaterialProperty(bindDesc.Name, srv, PROPERTY_BUFFER, dimension));

				}

				else if (bindDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MipLevels = 1;

					if (bindDesc.ReturnType == D3D11_RETURN_TYPE_FLOAT)
					{
						if (bindDesc.uFlags == 1)						// 1 Channel
						{
							texture2dDesc.Format = DXGI_FORMAT_R32_FLOAT;
							srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
						}
						else if (bindDesc.uFlags <= 3)
						{
							texture2dDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
							srvDesc.Format = DXGI_FORMAT_R32G32_FLOAT;


						}
						else if (bindDesc.uFlags <= 7)
						{
							texture2dDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
							srvDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;


						}
						else if (bindDesc.uFlags <= 15)
						{
							texture2dDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
							srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

						}
					}
					else if (bindDesc.ReturnType == D3D11_RETURN_TYPE_UINT)
					{
						if (bindDesc.uFlags == 1)						// 1 Channel
						{
							texture2dDesc.Format = DXGI_FORMAT_R32_UINT;
							srvDesc.Format = DXGI_FORMAT_R32_UINT;

						}
						else if (bindDesc.uFlags <= 3)
						{
							texture2dDesc.Format = DXGI_FORMAT_R32G32_UINT;
							srvDesc.Format = DXGI_FORMAT_R32G32_UINT;

						}
						else if (bindDesc.uFlags <= 7)
						{
							texture2dDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
							srvDesc.Format = DXGI_FORMAT_R32G32B32_UINT;

						}
						else if (bindDesc.uFlags <= 15)
						{
							texture2dDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
							srvDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
						}
					}
					else if (bindDesc.ReturnType == D3D11_RETURN_TYPE_SINT)
					{
						if (bindDesc.uFlags == 1)						// 1 Channel
						{
							texture2dDesc.Format = DXGI_FORMAT_R32_SINT;
							srvDesc.Format = DXGI_FORMAT_R32_SINT;
						}
						else if (bindDesc.uFlags <= 3)
						{
							texture2dDesc.Format = DXGI_FORMAT_R32G32_SINT;
							srvDesc.Format = DXGI_FORMAT_R32G32_SINT;

						}
						else if (bindDesc.uFlags <= 7)
						{
							texture2dDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
							srvDesc.Format = DXGI_FORMAT_R32G32B32_SINT;

						}
						else if (bindDesc.uFlags <= 15)
						{
							texture2dDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
							srvDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
						}
					}
				}

				//else if(bindDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE3D)
				//{

				//}



			}

			if (bindDesc.Type == D3D_SIT_CBUFFER)
			{
				bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

				ID3D11ShaderReflectionConstantBuffer* buffer = reflection->GetConstantBufferByIndex(bindDesc.BindPoint);

				D3D11_SHADER_BUFFER_DESC shaderBufferDesc{};
				std::vector<const char*> variableNames;

				result = buffer->GetDesc(&shaderBufferDesc);
				if (result != S_OK)
				{
					Debug::Log("failed to getting descriptor for constant buffer.");
					return result;
				}

				bufferDesc.ByteWidth = sizeof(shaderBufferDesc.Size);

				for (unsigned int i = 0; i < shaderBufferDesc.Variables; i++)
				{
					ID3D11ShaderReflectionVariable* var = buffer->GetVariableByIndex(i);
					D3D11_SHADER_VARIABLE_DESC varDesc{};
					result = var->GetDesc(&varDesc);
					if (result != S_OK)
					{
						Debug::Log("failed to getting descriptor for constant buffer member.");
						return result;
					}

					variableNames.emplace_back(varDesc.Name);



				}

				ComPtr<ID3D11Buffer> constantBuffer;

				result = mDevice->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
				if (result != S_OK)
				{
					Debug::Log("invalid argument for creating constant buffer.");
					return result;
				}

				long long hash = MakeHash(shaderBufferDesc.Name);

				if (mInputConstantProperties.find(hash) != mInputConstantProperties.end())
				{
					continue;
				}

				MCP* constProperties = new MCP(shaderBufferDesc.Name, variableNames, constantBuffer, bufferDesc.ByteWidth, shaderBufferDesc.Variables);


				mInputConstantProperties.insert_or_assign(MakeHash(shaderBufferDesc.Name), constProperties);

			}

			inputRegisters.emplace_back(bindDesc);
		}

		//for (unsigned int i = 0; i < shaderDesc.OutputParameters; i++)
		//{
		//	D3D11_SIGNATURE_PARAMETER_DESC outParam{};

		//	reflection->GetOutputParameterDesc(i, &outParam);

		//	if (outParam.SystemValueType == D3D_NAME_TARGET)
		//	{
		//	//	MOP* outputProperty = new MOP()
		//	}

		//}


		return S_OK;
	}

	Material::Material(Shader* shader, ID3D11Device* device, ID3D11DeviceContext* context)
		: mShader(shader), mDevice(device), mContext(context)
	{
		reflectVertex(shader->mVertexReflection.Get());
		reflectPixel(shader->mPixelReflection.Get());

		D3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC();;

		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;

		HRESULT result = mDevice->CreateRasterizerState(&rasterizerDesc, mRasterState.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("invalid argument during creating rasterizer state.");
			return;
		}
	}
	Material::~Material()
	{
	}
}