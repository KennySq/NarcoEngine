#pragma once
#include"Common.h"
#include<atlbase.h>
#include<d3dcompiler.h>

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

		ID3D11ShaderResourceView* const * GetVertexSRV() const { return mVertexShaderResources.data(); }

		void Release();
	private:
		eShaderFlag mFlags;

		std::string mPath;

		std::vector<ID3D11Buffer*> mVertexBuffers;
		std::vector<ID3D11Buffer*> mGeometryBuffers;
		std::vector<ID3D11Buffer*> mDomainBuffers;
		std::vector<ID3D11Buffer*> mHullBuffers;
		std::vector<ID3D11Buffer*> mPixelBuffers;
		
		std::vector<ID3D11ShaderResourceView*> mVertexShaderResources;
		std::vector<ID3D11ShaderResourceView*> mGeometryShaderResources;
		std::vector<ID3D11ShaderResourceView*> mDomainShaderResources;
		std::vector<ID3D11ShaderResourceView*> mHullShaderResources;
		std::vector<ID3D11ShaderResourceView*> mPixelShaderResources;

		std::vector<ID3D11RenderTargetView*> mPixelRenderTargets;

		std::vector<ID3D11UnorderedAccessView*> mVertexUnorderedAccess;
		std::vector<ID3D11UnorderedAccessView*> mGeometryUnorderedAccess;
		std::vector<ID3D11UnorderedAccessView*> mDomainUnorderedAccess;
		std::vector<ID3D11UnorderedAccessView*> mHullUnorderedAccess;
		std::vector<ID3D11UnorderedAccessView*> mPixelUnorderedAccess;

		ComPtr<ID3D11VertexShader> mVertex = nullptr;
		ComPtr<ID3D11GeometryShader> mGeometry = nullptr;
		ComPtr<ID3D11DomainShader> mDomain = nullptr;
		ComPtr<ID3D11HullShader> mHull = nullptr;
		ComPtr<ID3D11PixelShader> mPixel = nullptr;
	};
}