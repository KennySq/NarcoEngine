#pragma once
#include"Common.h"
#include<atlbase.h>
#include<d3dcompiler.h>
#include<d3d11shader.h>

#pragma comment(lib, "d3dcompiler.lib")

// #########################################################
// entry
//	- VS -> vert
//	- GS -> geom
//	- DS -> doma
//  - HS -> hull
//	- PS -> frag
// #########################################################

namespace NARCO
{
	enum eShaderFlag
	{
		SHADER_VERTEX = 1,
		SHADER_GEOMETRY = 2,
		SHADER_DOMAIN = 4,
		SHADER_HULL = 8,
		SHADER_PIXEL = 16,
		SHADER_COMPUTE = 0, // Independent
	};

	static constexpr unsigned int MODEL_5_IN_T_REGISTER = 128;
	static constexpr unsigned int MODEL_5_OUT_T_REGISTER = 8;
	static constexpr unsigned int MODEL_5_IN_B_REGISTER = 15;
	static constexpr unsigned int MODEL_5_IN_S_REGISTER = 16;
	static constexpr unsigned int MODEL_5_OUT_O_REGISTER = 32;

	class Material;
	class Shader
	{
		friend Material;
	public:
		Shader(const char* path, unsigned int flags);
		~Shader();

		HRESULT Compile(ID3D11Device* device);

		ID3D11VertexShader* GetVS() const { return mVertex.Get(); }
		ID3D11GeometryShader* GetGS() const { return mGeometry.Get(); }
		ID3D11DomainShader* GetDS() const { return mDomain.Get(); }
		ID3D11HullShader* GetHS() const { return mHull.Get(); }
		ID3D11PixelShader* GetPS() const { return mPixel.Get(); }
		ID3D11InputLayout* GetIL() const { return mLayout.Get(); }

		ID3D11ShaderReflection* GetVertexReflection() const { return mVertexReflection.Get(); }
		ID3D11ShaderReflection* GetGeometryReflection() const { return mGeometryReflection.Get(); }
		ID3D11ShaderReflection* GetDomainReflection() const { return mDomainReflection.Get(); }
		ID3D11ShaderReflection* GetHullReflection() const { return mHullReflection.Get(); }
		ID3D11ShaderReflection* GetPixelReflection() const { return mPixelReflection.Get(); }

		unsigned int GetVertexConstantBufferCount() const { return mVertexConstantBufferCount; }
		unsigned int GetVertexBoundResourceCount() const { return mVertexBoundResourceCount; }

		unsigned int GetPixelConstantBufferCount() const { return mPixelConstantBufferCount; }
		unsigned int GetPixelBoundResourceCount() const { return mPixelBoundResourceCount; }
		void Release();
	private:

		HRESULT reflectInputLayout(ID3D11Device* device, ID3DBlob* blob);

		eShaderFlag mFlags;

		std::string mPath;

		std::vector<ID3D11RenderTargetView*> mPixelRenderTargets;

		ComPtr<ID3D11VertexShader> mVertex = nullptr;
		ComPtr<ID3D11GeometryShader> mGeometry = nullptr;
		ComPtr<ID3D11DomainShader> mDomain = nullptr;
		ComPtr<ID3D11HullShader> mHull = nullptr;
		ComPtr<ID3D11PixelShader> mPixel = nullptr;
		ComPtr<ID3D11InputLayout> mLayout = nullptr;

		ComPtr<ID3D11ShaderReflection> mVertexReflection;
		ComPtr<ID3D11ShaderReflection> mGeometryReflection;
		ComPtr<ID3D11ShaderReflection> mDomainReflection;
		ComPtr<ID3D11ShaderReflection> mHullReflection;
		ComPtr<ID3D11ShaderReflection> mPixelReflection;

		std::vector<D3D11_SHADER_INPUT_BIND_DESC> mInputRegisters;

		unsigned int mVertexConstantBufferCount;
		unsigned int mVertexBoundResourceCount;

		unsigned int mGeometryConstantBufferCount;
		unsigned int mGeometryBoundResourceCount;

		unsigned int mDomainConstantBufferCount;
		unsigned int mDomainBoundResourceCount;

		unsigned int mHullConstantBufferCount;
		unsigned int mHullBoundResourceCount;

		unsigned int mPixelConstantBufferCount;
		unsigned int mPixelBoundResourceCount;
	};
}