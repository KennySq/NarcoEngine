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
			Material* material = mMaterials[i];
			const Shader* shader = mMaterials[i]->GetShader();
			ID3D11VertexShader* vs = shader->GetVS();
			ID3D11GeometryShader* gs = shader->GetGS();
			ID3D11DomainShader* ds = shader->GetDS();
			ID3D11HullShader* hs = shader->GetHS();
			ID3D11PixelShader* ps = shader->GetPS();
			
			static ID3D11Buffer* vertex[] = { mMesh->GetVertex() };
			static ID3D11Buffer* index = mMesh->GetIndex();
			static unsigned int strides[] = { mMesh->GetStride() };
			static unsigned int offsets[] = { 0 };

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

			auto textureRegisters = material->GetInputTextureRegisters();
			auto constantRegisters = material->GetInputConstantRegisters();

			unsigned int vertexConstantBuffers = shader->GetVertexConstantBufferCount();
			unsigned int vertexBoundResources = shader->GetVertexBoundResourceCount();
			
			mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			mContext->IASetVertexBuffers(0, 1, vertex, strides, offsets);
			mContext->IASetIndexBuffer(index, DXGI_FORMAT_R32_UINT, 0);
			mContext->IASetInputLayout(shader->GetIL());
			
			std::vector<ID3D11Buffer*> constantBuffers;

			for (auto itr = textureRegisters.begin(); itr != textureRegisters.end(); ++itr)
			{
				auto prop = itr->second;
				
				
			}

			for (auto itr = constantRegisters.begin(); itr != constantRegisters.end(); ++itr)
			{
				auto prop = itr->second;

				constantBuffers.emplace_back(prop->Constant.Get());
			}
			
			mContext->VSSetConstantBuffers(0, constantBuffers.size(), constantBuffers.data());
			mContext->VSSetShaderResources(0, )
			
		}

	}

	void Renderer::render(float delta)
	{
	}

	void Renderer::release()
	{
	}
}