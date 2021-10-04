#include"inc/Renderer.h"

namespace NARCO
{
	void Renderer::SetMaterial(Material* material)
	{
		if (material == nullptr)
		{
			ExceptionWarning(E_INVALIDARG, "material was nullptr.");
			return;
		}
		mMaterial = material;
	}
	void Renderer::SetMesh(Mesh* mesh)
	{
		if (mesh == nullptr)
		{
			ExceptionWarning(E_INVALIDARG, "mesh was nullptr.");
		}

		mMesh = mesh;
	}
	Renderer::Renderer()
		: Component(typeid(this).name()), mMesh(nullptr)
	{

	}
	Renderer::~Renderer()
	{
	}
	void Renderer::stage_UpdateResource(const Material* material)
	{
		std::map<long long, MP*> textureRegisters = material->GetInputTextureRegisters();
		std::map<long long, MCP*> constantRegisters = material->GetInputConstantRegisters();
		
		MCP* defaultConstProperty = constantRegisters.begin()->second;

		ID3D11Buffer* destBuffer = defaultConstProperty->Constant.Get();
		ID3D11Buffer* world = mTransform->GetBuffer();
		ID3D11Buffer* proj = mRenderCamera->GetProjectionBuffer();
		ID3D11Buffer* view = mRenderCamera->GetViewBuffer();

		unsigned int viewOffset = sizeof(float) * 16;
		unsigned int projOffset = sizeof(float) * 32;

		mContext->CopySubresourceRegion(destBuffer, 0, 0, 0, 0, world, 0, nullptr);
		mContext->CopySubresourceRegion(destBuffer, 0, viewOffset, 0, 0, view, 0, nullptr);
		mContext->CopySubresourceRegion(destBuffer, 0, projOffset, 0, 0, proj, 0, nullptr);
	}

	void Renderer::stage_InputAssembly(const Mesh* mesh, const Shader* shader)
	{
		ID3D11Buffer* vertex[] = { mesh->GetVertex() };
		ID3D11Buffer* index = mesh->GetIndex();
		unsigned int indexCount = mesh->GetIndexCount();

		unsigned int strides[] = { mesh->GetStride() };
		unsigned int offsets[] = { 0 };



		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mContext->IASetVertexBuffers(0, 1, vertex, strides, offsets);
		mContext->IASetIndexBuffer(index, DXGI_FORMAT_R32_UINT, 0);
		mContext->IASetInputLayout(shader->GetIL());

	}
	void Renderer::stage_Vertex(const Material* material, const Shader* shader)
	{
		ID3D11VertexShader* vs = shader->GetVS();

		unsigned int constBufferCount = mVertexConstantBuffers.size();
		unsigned int textureCount = mVertexTextures.size();

		ID3D11Buffer* const* constBuffers = mVertexConstantBuffers.data();
		ID3D11ShaderResourceView* const* textures = mVertexTextures.data();

		if (vs != nullptr)
		{
			mContext->VSSetShader(vs, nullptr, 0);

			mContext->VSSetConstantBuffers(0, constBufferCount, constBuffers);
			mContext->VSSetShaderResources(0, textureCount, textures);
		}
	}
	void Renderer::stage_Geometry(const Material* material, const Shader* shader)
	{
		ID3D11GeometryShader* gs = shader->GetGS();

		if (gs != nullptr)
		{
			mContext->GSSetShader(gs, nullptr, 0);
		}

	}
	void Renderer::stage_Domain(const Material* material, const Shader* shader)
	{
		ID3D11DomainShader* ds = shader->GetDS();

		if (ds != nullptr)
		{
			mContext->DSSetShader(ds, nullptr, 0);
		}

	}
	void Renderer::stage_Hull(const Material* material, const Shader* shader)
	{
		ID3D11HullShader* hs = shader->GetHS();

		if (hs != nullptr)
		{
			mContext->HSSetShader(hs, nullptr, 0);
		}
	}
	void Renderer::stage_Pixel(const Material* material, const Shader* shader)
	{
		ID3D11PixelShader* ps = shader->GetPS();

		unsigned int constCount = mPixelConstantBuffers.size();
		unsigned int textureCount = mPixelTextures.size();
		
		ID3D11Buffer* const* constBuffers = mPixelConstantBuffers.data();
		ID3D11ShaderResourceView* const* textures = mPixelTextures.data();

		if (ps != nullptr)
		{
			mContext->PSSetShader(ps, nullptr, 0);

			mContext->PSSetConstantBuffers(0, constCount, constBuffers);
			mContext->PSSetShaderResources(0, textureCount, textures);
		}
	}

	void Renderer::stage_ReflectVertex(const Material* material, const Shader* shader)
	{
	}

	void Renderer::stage_ReflectGeometry(const Material* material, const Shader* shader)
	{
	}

	void Renderer::stage_ReflectDomain(const Material* material, const Shader* shader)
	{
	}

	void Renderer::stage_ReflectHull(const Material* material, const Shader* shader)
	{
	}

	void Renderer::stage_ReflectPixel(const Material* material, const Shader* shader)
	{
	}

	void Renderer::awake()
	{
		const Scene* scene = mRoot->GetScene();
		mContext = scene->GetContext();
		mRenderCamera = scene->FindGameObjectWithTag("Main Camera")->GetComponent<Camera>();

		mTransform = mRoot->GetComponent<Transform>();

	}

	void Renderer::start()
	{
		const Scene* scene = mRoot->GetScene();

		mTransform = mRoot->GetComponent<Transform>();

		ID3D11Buffer* transformBuffer = mTransform->GetBuffer();
		ID3D11Buffer* viewBuffer = mRenderCamera->GetViewBuffer();
		ID3D11Buffer* projectionBuffer = mRenderCamera->GetProjectionBuffer();

		ID3D11Buffer* cameraBuffers[] = { viewBuffer, projectionBuffer };

		Narco_Deferred_Legacy* rp = scene->GetRP();
		GBuffer* gbuffer = rp->GetGBuffer();

		auto shader = mMaterial->GetShader();
		auto vertReflection = shader->GetVertexReflection();
		auto geoReflection = shader->GetGeometryReflection();
		auto domReflection = shader->GetDomainReflection();
		auto hulReflection = shader->GetHullReflection();
		auto pixReflection = shader->GetPixelReflection();

		auto constBufferProperties = mMaterial->GetInputConstantRegisters();
		auto textureProperties = mMaterial->GetInputTextureRegisters();

		D3D11_SHADER_DESC vertShaderDesc{}, geoShaderDesc{}, domShaderDesc{}, hulShaderDesc{}, pixShaderDesc{};
		D3D11_SHADER_BUFFER_DESC vertCbDesc{}, geoCbDesc{}, domCbDesc{}, hulCbDesc{}, pixCbDesc{};
		D3D11_SHADER_INPUT_BIND_DESC vertTexDesc{}, geoTexDesc{}, domTexDesc{}, hullTexDesc{}, pixTexDesc{};
		// Vertex Stage
		if (vertReflection != nullptr)
		{
			vertReflection->GetDesc(&vertShaderDesc);

			unsigned int vertCbCount = vertShaderDesc.ConstantBuffers;
			unsigned int vertTexCount = vertShaderDesc.BoundResources;

			unsigned int index = 0;

			for (unsigned int i = 0; i < vertCbCount; i++)
			{
				auto vertCbRf = vertReflection->GetConstantBufferByIndex(i);
				vertCbRf->GetDesc(&vertCbDesc);

				auto lookup = constBufferProperties.find(MakeHash(vertCbDesc.Name));

				if (lookup == constBufferProperties.end())
				{
					continue;
				}

				MCP* reg = lookup->second;
				mVertexConstantBuffers.emplace_back(reg->Constant.Get());
			}

			for (unsigned int i = 0; i < vertTexCount; i++)
			{
				vertReflection->GetResourceBindingDesc(i, &vertTexDesc);

				if (vertTexDesc.Type == D3D_SIT_CBUFFER)
				{
					continue;
				}

				auto lookup = textureProperties.find(MakeHash(vertTexDesc.Name));

				if (lookup == textureProperties.end())
				{
					continue;
				}

				MP* reg = lookup->second;
				mVertexTextures.emplace_back(reg->Register.Get());
			}

			if (vertCbCount > 0)
			{
				mContext->VSSetConstantBuffers(0, vertCbCount, mVertexConstantBuffers.data());
			}
			if (vertTexCount > 0)
			{
				mContext->VSSetShaderResources(0, vertTexCount - vertCbCount, mVertexTextures.data());
			}
		}

		// Pixel Stage
		if (pixReflection != nullptr)
		{
			pixReflection->GetDesc(&pixShaderDesc);

			unsigned int pixCbCount = pixShaderDesc.ConstantBuffers;
			unsigned int pixTexCount = pixShaderDesc.BoundResources;
			unsigned int pixOutCount = pixShaderDesc.OutputParameters;
			//unsigned int pixSmpCount = pixShaderDesc.Sample
			for (unsigned int i = 0; i < pixCbCount; i++)
			{
				auto pixCbRf = pixReflection->GetConstantBufferByIndex(i);
				pixCbRf->GetDesc(&pixCbDesc);

				auto lookup = constBufferProperties.find(MakeHash(pixCbDesc.Name));

				if (lookup == constBufferProperties.end())
				{
					continue;
				}

				MCP* reg = lookup->second;
				mGeometryConstantBuffers.emplace_back(reg->Constant.Get());
			}

			for (unsigned int i = 0; i < pixTexCount; i++)
			{
				pixReflection->GetResourceBindingDesc(i, &pixTexDesc);

				if (pixTexDesc.Type == D3D_SIT_CBUFFER)
				{
					continue;
				}

				if (pixTexDesc.Dimension == D3D_SRV_DIMENSION_BUFFER)
				{
					continue;
				}

				auto lookup = textureProperties.find(MakeHash(pixTexDesc.Name));

				if (lookup == textureProperties.end())
				{
					continue;
				}

				MP* reg = lookup->second;
				mPixelTextures.emplace_back(reg->Register.Get());
			}

			for (unsigned int i = 0; i < pixOutCount; i++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC outParam{};

				pixReflection->GetOutputParameterDesc(i, &outParam);
			
				
				
			}

			mContext->OMSetRenderTargets(gbuffer->GetBufferCount(), gbuffer->GetRenderTargets(), gbuffer->GetDepthStencil());


			if (pixCbCount > 0)
			{
				mContext->PSSetConstantBuffers(0, pixCbCount, mPixelConstantBuffers.data());
			}
			if (pixTexCount > 0)
			{
				mContext->PSSetShaderResources(0, mPixelTextures.size(), mPixelTextures.data());
				
			}



		}





	}

	void Renderer::update(float delta)
	{
		static ID3D11ShaderResourceView* nullSrv[] = { nullptr };
		static ID3D11RenderTargetView* nullRtv[] = { nullptr };

		const Material* material = mMaterial;
		const Shader* shader = mMaterial->GetShader();
		const Mesh* mesh = mMesh;
		ID3D11VertexShader* vs = shader->GetVS();
		ID3D11GeometryShader* gs = shader->GetGS();
		ID3D11DomainShader* ds = shader->GetDS();
		ID3D11HullShader* hs = shader->GetHS();
		ID3D11PixelShader* ps = shader->GetPS();
		ID3D11RasterizerState* rsState = material->GetRasterizerState();

		unsigned int indexCount = mesh->GetIndexCount();

		stage_UpdateResource(material);
		stage_InputAssembly(mesh, shader);
		stage_Vertex(material, shader);
		stage_Geometry(material, shader);
		stage_Domain(material, shader);
		stage_Hull(material, shader);
		stage_Pixel(material, shader);

		mContext->RSSetState(rsState);
		
		mContext->DrawIndexed(indexCount, 0, 0);

		mContext->PSSetShaderResources(0, 1, nullSrv);
		mContext->OMSetRenderTargets(1, nullRtv, nullptr);
	}

	void Renderer::render(float delta)
	{
	}

	void Renderer::release()
	{
	}

}