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

	class Shader
	{
	public:
		Shader(const char* path, unsigned int flags);
		~Shader();

		HRESULT Compile(ID3D11Device* device);

		ID3D11VertexShader* GetVS() const { return mVertex.Get(); }
		ID3D11GeometryShader* GetGS() const { return mGeometry.Get(); }
		ID3D11DomainShader* GetDS() const { return mDomain.Get(); }
		ID3D11HullShader* GetHS() const { return mHull.Get(); }
		ID3D11PixelShader* GetPS() const { return mPixel.Get(); }

		//ID3D11ShaderResourceView* const * GetVertexSRV() const { return mVertexShaderResources.data(); }

		const std::vector<D3D11_SHADER_INPUT_BIND_DESC>& GetInputRegisters() const { return mInputRegisters; }

		void Release();
	private:
		HRESULT reflectVertex();
		HRESULT reflectPixel();

		HRESULT reflectInputLayout(ID3D11Device* device, ID3DBlob* blob);

		eShaderFlag mFlags;

		std::string mPath;

		std::vector<ID3D11Buffer*> mConstantBuffers; // 8 Max

		std::vector<ID3D11RenderTargetView*> mPixelRenderTargets;

		std::vector<ID3D11ShaderResourceView*> mShaderResources;

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
	};
}