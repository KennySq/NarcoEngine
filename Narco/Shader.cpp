#include"inc/Shader.h"

namespace NARCO
{
	Shader::Shader(const char* path, unsigned int flags)
		: mPath(path), mFlags((eShaderFlag)flags)
	{
		
	}
	Shader::~Shader()
	{
	}
	HRESULT Shader::Compile(ID3D11Device* device)
	{
		USES_CONVERSION;
		
		HRESULT result;
		DWORD compileFlag = 0;

		Release();

#ifdef _DEBUG
		compileFlag |= D3DCOMPILE_DEBUG;
#endif
		const char* path = mPath.c_str();
		ID3DBlob* blob, * errBlob;

		if ((mFlags & SHADER_VERTEX))
		{
			result = D3DCompileFromFile(A2W(path), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vert", "vs_5_0", 0, compileFlag, &blob, &errBlob);
			if (result != S_OK)
			{
				ExceptionError(result, reinterpret_cast<const char*>(errBlob->GetBufferPointer()));
				errBlob->Release();

				return result;
			}
			result = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(mVertexReflection.GetAddressOf()));
			if (result != S_OK)
			{
				ExceptionError(result, reinterpret_cast<const char*>(errBlob->GetBufferPointer()));

				errBlob->Release();

				return result;
			}

			result = reflectInputLayout(device, blob);


			result = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mVertex.GetAddressOf());
			ExceptionError(result, "Creating vertex shader");

			blob->Release();
		}

		if ((mFlags & SHADER_GEOMETRY))
		{
			result = D3DCompileFromFile(A2W(path), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "geom", "gs_5_0", 0, compileFlag, &blob, &errBlob);

			if (result != S_OK)
			{
				ExceptionError(result, reinterpret_cast<const char*>(errBlob->GetBufferPointer()));
				errBlob->Release();

				return result;
			}

			result = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(mGeometryReflection.GetAddressOf()));

			if (result != S_OK)
			{
				ExceptionError(result, reinterpret_cast<const char*>(errBlob->GetBufferPointer()));

				errBlob->Release();

				return result;
			}

			result = device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mGeometry.GetAddressOf());
			ExceptionError(result, "Creating geometry shader");

			blob->Release();
		}

		if ((mFlags & SHADER_DOMAIN))
		{
			result = D3DCompileFromFile(A2W(path), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "doma", "ds_5_0", 0, compileFlag, &blob, &errBlob);

			if (result != S_OK)
			{
				ExceptionError(result, reinterpret_cast<const char*>(errBlob->GetBufferPointer()));
				errBlob->Release();
				return result;
			}

			result = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(mDomainReflection.GetAddressOf()));

			if (result != S_OK)
			{
				ExceptionError(result, reinterpret_cast<const char*>(errBlob->GetBufferPointer()));

				errBlob->Release();

				return result;
			}

			result = device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mDomain.GetAddressOf());
			ExceptionError(result, "Creating domain shader");

			blob->Release();
		}

		if ((mFlags & SHADER_HULL))
		{
			result = D3DCompileFromFile(A2W(path), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "hull", "hs_5_0", 0, compileFlag, &blob, &errBlob);

			if (result != S_OK)
			{
				ExceptionError(result, reinterpret_cast<const char*>(errBlob->GetBufferPointer()));
				errBlob->Release();

				return result;
			}

			result = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(mHullReflection.GetAddressOf()));

			if (result != S_OK)
			{
				ExceptionError(result, reinterpret_cast<const char*>(errBlob->GetBufferPointer()));

				errBlob->Release();

				return result;
			}

			result = device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mHull.GetAddressOf());
			ExceptionError(result, "Creating hull shader");

			blob->Release();
		}

		if ((mFlags & SHADER_PIXEL))
		{
			result = D3DCompileFromFile(A2W(path), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "frag", "ps_5_0", 0, compileFlag, &blob, &errBlob);

			if (result != S_OK)
			{
				ExceptionError(result, reinterpret_cast<const char*>(errBlob->GetBufferPointer()));
				return result;
			}

			result = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(mPixelReflection.GetAddressOf()));

			if (result != S_OK)
			{
				ExceptionError(result, reinterpret_cast<const char*>(errBlob->GetBufferPointer()));

				errBlob->Release();

				return result;
			}

			result = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mPixel.GetAddressOf());
			ExceptionError(result, "Creating pixel shader");

			blob->Release();
		}

		reflectVertex();
		reflectPixel();

		return S_OK;
	}
	void Shader::Release()
	{
		if (mVertex != nullptr)
		{
			mVertex->Release();
		}

		if (mGeometry != nullptr)
		{
			mGeometry->Release();
		}

		if (mDomain != nullptr)
		{
			mDomain->Release();
		}

		if (mHull != nullptr)
		{
			mHull->Release();
		}

		if (mPixel != nullptr)
		{
			mPixel->Release();
		}
	}
	HRESULT Shader::reflectVertex()
	{
		HRESULT result;
		D3D11_SHADER_DESC shaderDesc{};

		result = mVertexReflection->GetDesc(&shaderDesc);

		if (result != S_OK)
		{
			ExceptionError(E_FAIL, "Faield to getting descriptor from vertex reflection");
			return result;
		}

		std::vector<D3D11_SIGNATURE_PARAMETER_DESC> inputParameters;
		std::vector<D3D11_SIGNATURE_PARAMETER_DESC> outputParameters;

		for (unsigned int i = 0; i < shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC parameterDesc{};
			result = mVertexReflection->GetInputParameterDesc(i, &parameterDesc);

			if (result != S_OK)
			{
				break;
			}

			inputParameters.emplace_back(parameterDesc);

		}

		for (unsigned int i = 0; i < shaderDesc.OutputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC parameterDesc{};
			result = mVertexReflection->GetOutputParameterDesc(i, &parameterDesc);

			if (result != S_OK)
			{
				break;
			}

			outputParameters.emplace_back(parameterDesc);
		}

		for (unsigned int i = 0; i < shaderDesc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC bindDesc{};
			result = mVertexReflection->GetResourceBindingDesc(i, &bindDesc);
			if (result != S_OK)
			{
				ExceptionError(E_FAIL, "failed to getting bound resource register");
				return E_FAIL;
			}

			mInputRegisters.emplace_back(bindDesc);
		}

		return S_OK;
	}
	HRESULT Shader::reflectPixel()
	{
		HRESULT result;
		D3D11_SHADER_DESC shaderDesc{};
		std::vector<D3D11_SHADER_INPUT_BIND_DESC> inputRegisters;

		result = mPixelReflection->GetDesc(&shaderDesc);

		for (unsigned int i = 0; i < shaderDesc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC bindDesc{};
			result = mPixelReflection ->GetResourceBindingDesc(i, &bindDesc);
			if (result != S_OK)
			{
				ExceptionError(E_FAIL, "failed to getting bound resource register");
				return E_FAIL;
			}

			inputRegisters.emplace_back(bindDesc);
		}

		return S_OK;
	}
	HRESULT Shader::reflectInputLayout(ID3D11Device* device, ID3DBlob* blob)
	{
		HRESULT result;
		D3D11_SHADER_DESC shaderDesc{};

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;

		result = mVertexReflection->GetDesc(&shaderDesc);

		for (unsigned int i = 0; i < shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC parameterDesc{};

			mVertexReflection->GetInputParameterDesc(i, &parameterDesc);

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

		result = device->CreateInputLayout(inputElements.data(), inputElements.size(), blob->GetBufferPointer(), blob->GetBufferSize(), mLayout.GetAddressOf());
		if (result != S_OK)
		{
			ExceptionError(result, "failed to create input layout");
			return result;
		}

		return S_OK;
	}
}