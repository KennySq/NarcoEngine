#include"inc/Renderer.h"

namespace NARCO
{
	void Renderer::SetMaterial(Material* material)
	{
		if (material == nullptr)
		{
			Debug::Log(mName + " material is nullptr.");
			return;
		}
		mMaterial = material;
	}
	void Renderer::SetMesh(Mesh* mesh)
	{
		if (mesh == nullptr)
		{
			Debug::Log(mName + " mesh is nullptr.");
		}

		mMesh = mesh;
	}
	Renderer::Renderer()
		: mMesh(nullptr)
	{

	}
	Renderer::~Renderer()
	{
	}
	//void Renderer::stage_UpdateResource(const Material* material, const Shader* shader)
	//{
	//	


	//	// ...



	//	std::map<long long, MP*> textureRegisters = material->GetInputTextureRegisters();
	//	std::map<long long, MCP*> constantRegisters = material->GetInputConstantRegisters();
	//	
	//	MCP* defaultConstProperty = constantRegisters.begin()->second;

	//	ID3D11Buffer* destBuffer = defaultConstProperty->Buffer.Get();
	//	ID3D11Buffer* world = mTransform->GetBuffer();
	//	ID3D11Buffer* proj = mRenderCamera->GetProjectionBuffer();
	//	ID3D11Buffer* view = mRenderCamera->GetViewBuffer();

	//	unsigned int viewOffset = sizeof(float) * 16;
	//	unsigned int projOffset = sizeof(float) * 32;

	//	mContext->CopySubresourceRegion(destBuffer, 0, 0, 0, 0, world, 0, nullptr);
	//	mContext->CopySubresourceRegion(destBuffer, 0, viewOffset, 0, 0, view, 0, nullptr);
	//	mContext->CopySubresourceRegion(destBuffer, 0, projOffset, 0, 0, proj, 0, nullptr);
	//}

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
		Reflector* vertRef = shader->GetVertexReflect();

		const auto& constBuffers = vertRef->GetBuffers();
		const auto& textures = vertRef->GetSRV();

		unsigned int constBufferCount = constBuffers.size();
		unsigned int textureCount = textures.size();

		ID3D11Buffer* const* rawConstBuffers = constBuffers.data();
		ID3D11ShaderResourceView* const* rawTextures = textures.data();

		if (vs != nullptr)
		{
			mContext->VSSetShader(vs, nullptr, 0);

			mContext->VSSetConstantBuffers(0, constBufferCount, rawConstBuffers);
			mContext->VSSetShaderResources(0, textureCount, rawTextures);
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

		Reflector* pixRef = shader->GetPixelReflect();

		const auto& constBuffers = pixRef->GetBuffers();
		const auto& textures = pixRef->GetSRV();

		unsigned int constBufferCount = constBuffers.size();
		unsigned int textureCount = textures.size();
		
		ID3D11Buffer* const* rawConstBuffers = constBuffers.data();
		ID3D11ShaderResourceView* const* rawTextures = textures.data();

		if (ps != nullptr)
		{
			mContext->PSSetShader(ps, nullptr, 0);

			mContext->PSSetConstantBuffers(0, constBufferCount, rawConstBuffers);
			mContext->PSSetShaderResources(0, textureCount, rawTextures);
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
		auto vertReflect = shader->GetVertexReflect();
		auto geoReflect = shader->GetGeometryReflect();
		auto domReflect = shader->GetDomainReflect();
		auto hulReflect = shader->GetHullReflect();
		auto pixReflect = shader->GetPixelReflect();

		const auto& vertConsts = vertReflect->GetBuffers();
		const auto& vertTextures = vertReflect->GetSRV();
		const auto& verUnorderes = vertReflect->GetUAV();

		const auto& geoConsts = geoReflect->GetBuffers();
		const auto& geoTextures = geoReflect->GetSRV();
		const auto& geoUnorders = geoReflect->GetUAV();

		const auto& domConsts = domReflect->GetBuffers();
		const auto& domTextures = domReflect->GetSRV();
		const auto& domUnorders = domReflect->GetUAV();

		const auto& hulConsts = hulReflect->GetBuffers();
		const auto& hulTextures = hulReflect->GetSRV();
		const auto& hulUnorders = hulReflect->GetUAV();

		const auto& pixConsts = pixReflect->GetBuffers();
		const auto& pixTextures = pixReflect->GetSRV();
		const auto& pixUnorders = pixReflect->GetUAV();

		if (vertReflect != nullptr)
		{
			uint constCount = vertReflect->GetConstBufferCount();
			uint resourceCount = vertReflect->GetBoundResourceCount();
			uint uaCount = vertReflect->GetUnorderedCount();
			uint sampCount = vertReflect->GetSamplerCount();

			for (uint i = 0; i < constCount; i++)
			{
				MCP* mcp = vertReflect->ReflectConstantBuffer(mDevice, i);
			
				if (mcp != nullptr)
				{
					vertReflect->AddBuffer(mcp);
				}
			}

			for (uint i = 0; i < resourceCount; i++)
			{
				MP* mp = vertReflect->ReflectTexture(mDevice, i);

				if (mp != nullptr)
				{
					vertReflect->AddSRV(mp);
				}
			}

		}


		if (vertReflect != nullptr)
		{

			for (auto& i : vertConsts) { mMaterial->AddBuffer(i); }
			for (auto& i : vertTextures) { mMaterial->AddTexture(i); }

		}
		if (geoReflect != nullptr)
		{
			for (auto& i : geoConsts) { mMaterial->AddBuffer(i); }
			for (auto& i : geoTextures) { mMaterial->AddTexture(i); }

		}
		if (domReflect != nullptr)
		{
			for (auto& i : domConsts) { mMaterial->AddBuffer(i); }
			for (auto& i : domTextures) { mMaterial->AddTexture(i); }

		}
		if (hulReflect != nullptr)
		{
			for (auto& i : hulConsts) { mMaterial->AddBuffer(i); }
			for (auto& i : hulTextures) { mMaterial->AddTexture(i); }

		}

		if (pixReflect != nullptr)
		{
			for (auto& i : pixConsts) { mMaterial->AddBuffer(i); }
			for (auto& i : pixTextures) { mMaterial->AddTexture(i); }

		}
		
		
		
		//if (vertReflection != nullptr)
		//{
		//	vertReflection->GetDesc(&vertShaderDesc);

		//	unsigned int vertCbCount = vertShaderDesc.ConstantBuffers;
		//	unsigned int vertTexCount = vertShaderDesc.BoundResources;

		//	unsigned int index = 0;

		//	for (unsigned int i = 0; i < vertCbCount; i++)
		//	{
		//		auto vertCbRf = vertReflection->GetConstantBufferByIndex(i);
		//		vertCbRf->GetDesc(&vertCbDesc);

		//		auto lookup = constBufferProperties.find(MakeHash(vertCbDesc.Name));

		//		if (lookup == constBufferProperties.end())
		//		{
		//			continue;
		//		}

		//		MCP* reg = lookup->second;
		//		mVertexConstantBuffers.emplace_back(reg->Buffer.Get());
		//	}

		//	for (unsigned int i = 0; i < vertTexCount; i++)
		//	{
		//		vertReflection->GetResourceBindingDesc(i, &vertTexDesc);

		//		if (vertTexDesc.Type == D3D_SIT_CBUFFER)
		//		{
		//			continue;
		//		}

		//		auto lookup = textureProperties.find(MakeHash(vertTexDesc.Name));

		//		if (lookup == textureProperties.end())
		//		{
		//			continue;
		//		}

		//		MP* reg = lookup->second;
		//		mVertexTextures.emplace_back(reg->Register.Get());
		//	}

		//	if (vertCbCount > 0)
		//	{
		//		mContext->VSSetConstantBuffers(0, vertCbCount, mVertexConstantBuffers.data());
		//	}
		//	if (vertTexCount > 0)
		//	{
		//		mContext->VSSetShaderResources(0, vertTexCount - vertCbCount, mVertexTextures.data());
		//	}
		//}

		//// Pixel Stage
		//if (pixReflection != nullptr)
		//{
		//	pixReflection->GetDesc(&pixShaderDesc);

		//	unsigned int pixCbCount = pixShaderDesc.ConstantBuffers;
		//	unsigned int pixTexCount = pixShaderDesc.BoundResources;
		//	unsigned int pixOutCount = pixShaderDesc.OutputParameters;
		//	//unsigned int pixSmpCount = pixShaderDesc.Sample
		//	for (unsigned int i = 0; i < pixCbCount; i++)
		//	{
		//		auto pixCbRf = pixReflection->GetConstantBufferByIndex(i);
		//		pixCbRf->GetDesc(&pixCbDesc);

		//		auto lookup = constBufferProperties.find(MakeHash(pixCbDesc.Name));

		//		if (lookup == constBufferProperties.end())
		//		{
		//			continue;
		//		}

		//		MCP* reg = lookup->second;
		//		mGeometryConstantBuffers.emplace_back(reg->Buffer.Get());
		//	}

		//	for (unsigned int i = 0; i < pixTexCount; i++)
		//	{
		//		pixReflection->GetResourceBindingDesc(i, &pixTexDesc);

		//		if (pixTexDesc.Type == D3D_SIT_CBUFFER)
		//		{
		//			continue;
		//		}

		//		if (pixTexDesc.Dimension == D3D_SRV_DIMENSION_BUFFER)
		//		{
		//			continue;
		//		}

		//		auto lookup = textureProperties.find(MakeHash(pixTexDesc.Name));

		//		if (lookup == textureProperties.end())
		//		{
		//			continue;
		//		}

		//		MP* reg = lookup->second;
		//		mPixelTextures.emplace_back(reg->Register.Get());
		//	}

		//	for (unsigned int i = 0; i < pixOutCount; i++)
		//	{
		//		D3D11_SIGNATURE_PARAMETER_DESC outParam{};

		//		pixReflection->GetOutputParameterDesc(i, &outParam);
		//	
		//		
		//		
		//	}

		//	mContext->OMSetRenderTargets(gbuffer->GetBufferCount(), gbuffer->GetRenderTargets(), gbuffer->GetDepthStencil());


		//	if (pixCbCount > 0)
		//	{
		//		mContext->PSSetConstantBuffers(0, pixCbCount, mPixelConstantBuffers.data());
		//	}
		//	if (pixTexCount > 0)
		//	{
		//		mContext->PSSetShaderResources(0, mPixelTextures.size(), mPixelTextures.data());
		//		
		//	}



		//}





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

		//stage_UpdateResource(material, shader);
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