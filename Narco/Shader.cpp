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
			result = D3DCompileFromFile(A2W(path), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "vert", "vs_5_0", compileFlag, 0, &blob, &errBlob);
			if (result != S_OK)
			{
				Debug::Log(reinterpret_cast<const char*>(errBlob->GetBufferPointer()));
				errBlob->Release();

				return result;
			}

			result = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mVertex.GetAddressOf());
			if (result != S_OK)
			{
				Debug::Log("faield to vertex shader");
				blob->Release();

				return result;
			}
			mVertexRef = new Reflector(device, blob);

			mLayout = mVertexRef->ReflectInputLayout(device, blob);

			if (result != S_OK)
			{
				Debug::Log("failed to reflect input layout");
				return result;
			}



			blob->Release();
		}

		if ((mFlags & SHADER_GEOMETRY))
		{
			result = D3DCompileFromFile(A2W(path), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "geom", "gs_5_0", compileFlag, 0, &blob, &errBlob);

			if (result != S_OK)
			{
				Debug::Log(reinterpret_cast<const char*>(errBlob->GetBufferPointer()));
				errBlob->Release();

				return result;
			}

			result = device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mGeometry.GetAddressOf());
			
			if (result != S_OK)
			{
				Debug::Log("failed to create geometry shader");
				blob->Release();

				return result;
			}

			mGeometryRef = new Reflector(device, blob);

			blob->Release();
		}

		if ((mFlags & SHADER_DOMAIN))
		{
			result = D3DCompileFromFile(A2W(path), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "doma", "ds_5_0", compileFlag, 0, &blob, &errBlob);

			if (result != S_OK)
			{
				Debug::Log(reinterpret_cast<const char*>(errBlob->GetBufferPointer()));
				errBlob->Release();
				return result;
			}

			result = device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mDomain.GetAddressOf());
			if (result != S_OK)
			{
				Debug::Log("failed to create domain shader");

				blob->Release();

				return result;
			}

			mDomainRef = new Reflector(device, blob);

			blob->Release();
		}

		if ((mFlags & SHADER_HULL))
		{
			result = D3DCompileFromFile(A2W(path), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "hull", "hs_5_0", compileFlag, 0, &blob, &errBlob);

			if (result != S_OK)
			{
				Debug::Log(reinterpret_cast<const char*>(errBlob->GetBufferPointer()));
				errBlob->Release();

				return result;
			}

			result = device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mHull.GetAddressOf());
			if (result != S_OK)
			{
				Debug::Log("failed to create hull shader");
				blob->Release();
				return result;
			}

			mHullRef = new Reflector(device, blob);

			blob->Release();
		}

		if ((mFlags & SHADER_PIXEL))
		{
			result = D3DCompileFromFile(A2W(path), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "frag", "ps_5_0", compileFlag, 0, &blob, &errBlob);

			if (result != S_OK)
			{
				Debug::Log(reinterpret_cast<const char*>(errBlob->GetBufferPointer()));
				return result;
			}

			result = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mPixel.GetAddressOf());
			if (result != S_OK)
			{
				Debug::Log("failed to create pixel shader");

				blob->Release();
				return result;
			}

			mPixelRef = new Reflector(device, blob);

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