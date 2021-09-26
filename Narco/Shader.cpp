#include"inc/Shader.h"

namespace NARCO
{
	Shader::Shader(const char* path, unsigned int flags)
		: mPath(path), mFlags((eShaderFlag)flags),
		mVertexShaderResources(MODEL_5_IN_T_REGISTER),
		mVertexBuffers(MODEL_5_IN_B_REGISTER),
		mVertexUnorderedAccess(MODEL_5_OUT_O_REGISTER),
		mGeometryShaderResources(MODEL_5_IN_T_REGISTER),
		mGeometryBuffers(MODEL_5_IN_B_REGISTER),
		mGeometryUnorderedAccess(MODEL_5_OUT_O_REGISTER),
		mDomainShaderResources(MODEL_5_IN_T_REGISTER),
		mDomainBuffers(MODEL_5_IN_B_REGISTER),
		mDomainUnorderedAccess(MODEL_5_OUT_O_REGISTER),
		mHullShaderResources(MODEL_5_IN_T_REGISTER),
		mHullBuffers(MODEL_5_IN_B_REGISTER),
		mHullUnorderedAccess(MODEL_5_OUT_O_REGISTER),
		mPixelShaderResources(MODEL_5_IN_T_REGISTER),
		mPixelRenderTargets(MODEL_5_OUT_T_REGISTER),
		mPixelUnorderedAccess(MODEL_5_OUT_T_REGISTER)
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
				ExceptionError(result, reinterpret_cast<const char*>(errBlob));
				errBlob->Release();

				return result;
			}

			result = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mVertex.GetAddressOf());
			ExceptionError(result, "Creating vertex shader");

			blob->Release();
		}

		if ((mFlags & SHADER_GEOMETRY))
		{
			result = D3DCompileFromFile(A2W(path), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "geom", "gs_5_0", 0, compileFlag, &blob, &errBlob);

			if (result != S_OK)
			{
				ExceptionError(result, reinterpret_cast<const char*>(errBlob));
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
				ExceptionError(result, reinterpret_cast<const char*>(errBlob));
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
				ExceptionError(result, reinterpret_cast<const char*>(errBlob));
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
				ExceptionError(result, reinterpret_cast<const char*>(errBlob));
				return result;
			}

			result = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mPixel.GetAddressOf());
			ExceptionError(result, "Creating pixel shader");

			blob->Release();
		}

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
}