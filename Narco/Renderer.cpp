#include"inc/Renderer.h"

namespace NARCO
{
	void Renderer::AddMaterial(Material* material)
	{
		if (material == nullptr)
		{
			Debug::Log("invalid argument.");
			return;
		}

		mMaterials.emplace_back(material);
	}
	void Renderer::SetMaterial(Material* material, uint index)
	{
		if (index >= mMaterials.size() || index < 0)
		{
			Debug::Log("invalid index.");
			return;
		}

		if (material == nullptr)
		{
			Debug::Log(mName + " material is nullptr.");
			return;
		}
		mMaterials[index] = material;
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
	void Renderer::stage_UpdateReservedResources(const Material* material, const Shader* shader, uint materialID)
	{
		auto& bufferMap = material->GetBuffers();
		long long constantHash = MakeHash("Constants");

		struct CBuffer
		{
			XMFLOAT4X4 w, v, p;
			unsigned int matID[4];

		};

		ID3D11Buffer* worldBuffer = mTransform->GetBuffer();
		ID3D11Buffer* projectionBuffer = mRenderCamera->GetProjectionBuffer();
		ID3D11Buffer* viewBuffer = mRenderCamera->GetViewBuffer();


		auto constantItr = bufferMap.find(constantHash);
		MCP* constantProperty = constantItr->second;
		ID3D11Buffer* constantBuffer = constantProperty->Buffer.Get();

		CBuffer buffer;
		D3D11_MAPPED_SUBRESOURCE subData{};
		subData.pData = &buffer;

		mContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);

		XMStoreFloat4x4(&buffer.w, mTransform->GetMatrix());
		XMStoreFloat4x4(&buffer.v, mRenderCamera->GetView());
		XMStoreFloat4x4(&buffer.p, mRenderCamera->GetProjection());
		
		buffer.matID[0] = { materialID };

		mContext->Unmap(constantBuffer, 0);

		//mContext->CopySubresourceRegion(constantBuffer, 0, 0, 0, 0, worldBuffer, 0, nullptr);
		//mContext->CopySubresourceRegion(constantBuffer, 0, 64, 0, 0, viewBuffer, 0, nullptr);
		//mContext->CopySubresourceRegion(constantBuffer, 0, 128, 0, 0, projectionBuffer, 0, nullptr);
		//
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
		std::vector<ID3D11ShaderResourceView*> textures;

		ID3D11VertexShader* vs = shader->GetVS();
		Reflector* vertRef = shader->GetVertexReflect();

		const auto& constBuffers = vertRef->GetBuffers();
		const auto& shaderResourceMap = vertRef->GetSRVMap();
		//const auto& textures = vertRef->GetSRV();

		for (auto& s : shaderResourceMap)
		{
			ID3D11ShaderResourceView* srv = s.second->Register.Get();
			textures.emplace_back(srv);
		}

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

		textures.clear();
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
		static std::vector<ID3D11ShaderResourceView*> textures;

		ID3D11PixelShader* ps = shader->GetPS();

		Reflector* pixRef = shader->GetPixelReflect();

		const auto& constBuffers = pixRef->GetBuffers();

		const auto& shaderResourceMap = pixRef->GetSRVMap();

		unsigned int constBufferCount = constBuffers.size();
		//unsigned int textureCount = textures.size();
		
		ID3D11Buffer* const* rawConstBuffers = constBuffers.data();
		
		for (auto& s : shaderResourceMap)
		{
			ID3D11ShaderResourceView* srv = s.second->Register.Get();
			textures.emplace_back(srv);
		}

		uint rawTextureCount = textures.size();
		ID3D11ShaderResourceView* const* rawTextures = textures.data();

		if (ps != nullptr)
		{
			mContext->PSSetShader(ps, nullptr, 0);

			mContext->PSSetConstantBuffers(0, constBufferCount, rawConstBuffers);
			mContext->PSSetShaderResources(0, rawTextureCount, rawTextures);
		}

		textures.clear();

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
		static ID3D11Device* device = mDevice;
		const Scene* scene = mRoot->GetScene();

		mTransform = mRoot->GetComponent<Transform>();

		ID3D11Buffer* transformBuffer = mTransform->GetBuffer();
		ID3D11Buffer* viewBuffer = mRenderCamera->GetViewBuffer();
		ID3D11Buffer* projectionBuffer = mRenderCamera->GetProjectionBuffer();

		ID3D11Buffer* cameraBuffers[] = { viewBuffer, projectionBuffer };

		Narco_Deferred_Legacy* rp = scene->GetRP();
		GBuffer* gbuffer = rp->GetGBuffer();

		uint drawCount = mMaterials.size();

		for (uint index = 0; index < drawCount; index++)
		{
			auto shader = mMaterials[index]->GetShader();
			auto vertReflect = shader->GetVertexReflect();
			auto geoReflect = shader->GetGeometryReflect();
			auto domReflect = shader->GetDomainReflect();
			auto hulReflect = shader->GetHullReflect();
			auto pixReflect = shader->GetPixelReflect();

			const auto& vertConsts = vertReflect->GetBufferMap();
			const auto& vertTextures = vertReflect->GetSRVMap();
			const auto& verUnorderes = vertReflect->GetUAVMap();

			const auto& geoConsts = geoReflect->GetBufferMap();
			const auto& geoTextures = geoReflect->GetSRVMap();
			const auto& geoUnorders = geoReflect->GetUAVMap();

			const auto& domConsts = domReflect->GetBufferMap();
			const auto& domTextures = domReflect->GetSRVMap();
			const auto& domUnorders = domReflect->GetUAVMap();

			const auto& hulConsts = hulReflect->GetBufferMap();
			const auto& hulTextures = hulReflect->GetSRVMap();
			const auto& hulUnorders = hulReflect->GetUAVMap();

			const auto& pixConsts = pixReflect->GetBufferMap();
			const auto& pixTextures = pixReflect->GetSRVMap();
			const auto& pixUnorders = pixReflect->GetUAVMap();

			auto lamdaReflect = [](Reflector* reflect, ID3D11Device* device)
			{
				if (reflect != nullptr)
				{
					uint constCount = reflect->GetConstBufferCount();
					uint resourceCount = reflect->GetBoundResourceCount();
					uint uaCount = reflect->GetUnorderedCount();
					uint sampCount = reflect->GetSamplerCount();

					for (uint i = 0; i < constCount; i++)
					{


						MCP* mcp = reflect->ReflectConstantBuffer(device, i);
						
						if (mcp == nullptr)
						{
							continue;
						}
						
						if (reflect->Find(mcp->Name) == true)
						{
							delete mcp;
							continue;
						}

						if (mcp != nullptr)
						{
							reflect->AddBuffer(mcp);
						}
					}

					for (uint i = 0; i < resourceCount; i++)
					{
						MP* mp = reflect->ReflectTexture(device, i);

						if (mp == nullptr)
						{
							continue;
						}

						if (mp != nullptr)
						{
							reflect->AddSRV(mp);
						}
					}
				}
			};

			lamdaReflect(vertReflect, mDevice);
			lamdaReflect(pixReflect, mDevice);



			if (vertReflect != nullptr)
			{

				for (auto& i : vertConsts) { mMaterials[index]->AddBuffer(i.second); }
				for (auto& i : vertTextures) { mMaterials[index]->AddTexture(i.second); }

			}
			if (geoReflect != nullptr)
			{
				for (auto& i : geoConsts) { mMaterials[index]->AddBuffer(i.second); }
				for (auto& i : geoTextures) { mMaterials[index]->AddTexture(i.second); }

			}
			if (domReflect != nullptr)
			{
				for (auto& i : domConsts) { mMaterials[index]->AddBuffer(i.second); }
				for (auto& i : domTextures) { mMaterials[index]->AddTexture(i.second); }

			}
			if (hulReflect != nullptr)
			{
				for (auto& i : hulConsts) { mMaterials[index]->AddBuffer(i.second); }
				for (auto& i : hulTextures) { mMaterials[index]->AddTexture(i.second); }

			}

			if (pixReflect != nullptr)
			{
				for (auto& i : pixConsts) { mMaterials[index]->AddBuffer(i.second); }
				for (auto& i : pixTextures) { mMaterials[index]->AddTexture(i.second); }

			}

		}


	}

	void Renderer::update(float delta)
	{
		static ID3D11ShaderResourceView* nullSrv[1] = { nullptr };
		static ID3D11RenderTargetView* nullRtv[] = { nullptr };

		if (mMesh == nullptr)
		{
			return;
		}

		uint drawCount = mMaterials.size();
		for (uint index = 0; index < drawCount; index++)
		{
			//mContext->ClearState();

			const Material* material = mMaterials[index];
			const Shader* shader = mMaterials[index]->GetShader();
			const Mesh* mesh = mMesh;
			ID3D11VertexShader* vs = shader->GetVS();
			ID3D11GeometryShader* gs = shader->GetGS();
			ID3D11DomainShader* ds = shader->GetDS();
			ID3D11HullShader* hs = shader->GetHS();
			ID3D11PixelShader* ps = shader->GetPS();
			ID3D11RasterizerState* rsState = material->GetRasterizerState();

			unsigned int indexCount = mesh->GetIndexCount();

			const Scene* scene = mRoot->GetScene();
			Narco_Deferred_Legacy* rp = scene->GetRP();
			GBuffer* gbuffer = rp->GetGBuffer();

			mContext->OMSetRenderTargets(gbuffer->GetBufferCount(), gbuffer->GetRenderTargets(), gbuffer->GetDepthStencil());

			stage_UpdateReservedResources(material, shader, index);
			stage_InputAssembly(mesh, shader);
			stage_Vertex(material, shader);
			stage_Geometry(material, shader);
			stage_Domain(material, shader);
			stage_Hull(material, shader);
			stage_Pixel(material, shader);

			//mContext->RSSetState(rsState);

			mContext->DrawIndexed(indexCount, 0, 0);

			mContext->PSSetShaderResources(0, 1, &nullSrv[0]);
			mContext->OMSetRenderTargets(1, nullRtv, nullptr);
		}


	}

	void Renderer::render(float delta)
	{
	}

	void Renderer::release()
	{
	}

}