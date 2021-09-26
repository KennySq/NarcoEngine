#pragma once
#include"Common.h"
#include"Shader.h"

namespace NARCO
{
	class Material
	{
	public:
		Material();
		~Material();

		const Shader* GetShader() const { return mShader; }
		const std::string& GetPath() const { return mPath; }
		
		ID3D11RasterizerState* GetRasterizerState() const { return mRasterState.Get(); }
		ID3D11DepthStencilState* GetDepthStencilState() const { return mDepthState.Get(); }

	private:

		ComPtr<ID3D11RasterizerState> mRasterState;
		ComPtr<ID3D11DepthStencilState> mDepthState;

		std::string mPath;
		Shader* mShader;
	};
}