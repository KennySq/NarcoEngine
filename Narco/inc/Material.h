#pragma once
#include"Common.h"
#include"Shader.h"

namespace NARCO
{

	class Material
	{
	public:
		Material(Shader* shader, ID3D11Device* device, ID3D11DeviceContext* context);
		~Material();

		const Shader* GetShader() const { return mShader; }
		const std::string& GetPath() const { return mPath; }
		
		ID3D11RasterizerState* GetRasterizerState() const { return mRasterState.Get(); }
		ID3D11DepthStencilState* GetDepthStencilState() const { return mDepthState.Get(); }
		
		//const auto& GetInputConstantRegisters() const { return mInputConstantProperties; }
		//const auto& GetInputTextureRegisters() const { return mInputProperties; }

		//void UpdateBuffer(float* v, unsigned int count, long long hash)
		//{
		//	ID3D11Buffer* buffer;
		//	ID3D11ShaderResourceView* srv = mInputProperties[hash]->Register.Get();

		//	srv->GetResource(reinterpret_cast<ID3D11Resource**>(&buffer));

		//	mContext->UpdateSubresource(buffer, 0, nullptr, v, 0, 0);

		//	return;
		//}

		void AddBuffer(MCP* mcp);
		void AddTexture(MP* mp);
		void AddUnorder(MUP* mup);

		const auto& GetBuffers() const { return mConstants; }
		const auto& GetTextures() const { return mTextures; }
		const auto& GetUnorders() const { return mUnorders; }

		void UpdateConstant(void* data, long long hash)
		{
			auto result = mConstants.find(hash);
			if (result == mConstants.end())
			{
				Debug::Log("invalid index.");
				return;
			}

			mContext->UpdateSubresource(mConstants[hash]->Buffer.Get(), 0, nullptr, data, 0, 0);
		}

	private:

		//HRESULT reflectVertex(ID3D11ShaderReflection* reflection);
		//HRESULT reflectPixel(ID3D11ShaderReflection* reflection);

		ComPtr<ID3D11RasterizerState> mRasterState;
		ComPtr<ID3D11DepthStencilState> mDepthState;

		std::map<long long, MCP*> mConstants;
		std::map<long long, MP*> mTextures;
		std::map<long long, MUP*> mUnorders;
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mContext;
		std::string mPath;
		Shader* mShader;
		
	};
}