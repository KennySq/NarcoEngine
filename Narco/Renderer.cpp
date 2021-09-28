#include"inc/Renderer.h"

namespace NARCO
{
	Renderer::Renderer(ID3D11DeviceContext* context)
		: Component(typeid(this).name()), mContext(context), mMesh(nullptr)
	{
	}
	Renderer::~Renderer()
	{
	}
	void Renderer::start()
	{
		
	}

	void Renderer::update(float delta)
	{
		unsigned int materialCount = mMaterials.size();
		for (unsigned int i = 0; i < materialCount; i++)
		{
			const Shader* shader = mMaterials[i]->GetShader();
			ID3D11VertexShader* vs = shader->GetVS();
			ID3D11GeometryShader* gs = shader->GetGS();
			ID3D11DomainShader* ds = shader->GetDS();
			ID3D11HullShader* hs = shader->GetHS();
			ID3D11PixelShader* ps = shader->GetPS();
			
			if (vs != nullptr)
			{
				mContext->VSSetShader(vs, nullptr, 0);
			}

			if (gs != nullptr)
			{
				mContext->GSSetShader(gs, nullptr, 0);
			}
			
			if (ds != nullptr)
			{
				mContext->DSSetShader(ds, nullptr, 0);
			}

			if (hs != nullptr)
			{
				mContext->HSSetShader(hs, nullptr, 0);
			}
			
			if (ps != nullptr)
			{
				mContext->PSSetShader(ps, nullptr, 0);
			}

			//mContext->VSSetConstantBuffers(0, )

			//mContext->VSSetShaderResources(0, 1, shader->GetVertexSRV());
			//mContext->PSSetShaderResources(0,1, shader->Ge)
		}

	}

	void Renderer::render(float delta)
	{
	}

	void Renderer::release()
	{
	}
}