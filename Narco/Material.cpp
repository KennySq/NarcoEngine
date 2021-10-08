#include"inc/Material.h"

namespace NARCO
{

	Material::Material(Shader* shader, ID3D11Device* device, ID3D11DeviceContext* context)
		: mShader(shader), mDevice(device), mContext(context)
	{
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
	void Material::AddBuffer(MCP* mcp)
	{
		long long hash = MakeHash(mcp->Name);
		auto result = mConstants.find(hash);
		
		if (result == mConstants.end())
		{
			mConstants.insert_or_assign(hash, mcp);
		}

		return;
	}
	void Material::AddTexture(MP* mp)
	{
		long long hash = MakeHash(mp->Name);
		auto result = mTextures.find(hash);

		if (result == mTextures.end())
		{
			mTextures.insert_or_assign(hash, mp);
		}

		return;
	}
	void Material::AddUnorder(MUP* mup)
	{
		long long hash = MakeHash(mup->Name);
		auto result = mUnorders.find(hash);

		if (result != mUnorders.end())
		{
			mUnorders.insert_or_assign(hash, mup);
		}

		return;
	}
}