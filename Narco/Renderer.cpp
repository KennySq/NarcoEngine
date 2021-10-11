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
	void Renderer::stage_UpdateReservedResources(const Material* material, const Shader* shader)
	{
		static auto& bufferMap = material->GetBuffers();
		static long long constantHash = MakeHash("Constants");

		static ID3D11Buffer* worldBuffer = mTransform->GetBuffer();
		static ID3D11Buffer* projectionBuffer = mRenderCamera->GetProjectionBuffer();
		static ID3D11Buffer* viewBuffer = mRenderCamera->GetViewBuffer();


		auto constantItr = bufferMap.find(constantHash);
		MCP* constantProperty = constantItr->second;
		ID3D11Buffer* constantBuffer = constantProperty->Buffer.Get();

		mContext->CopySubresourceRegion(constantBuffer, 0, 0, 0, 0, worldBuffer, 0, nullptr);
		mContext->CopySubresourceRegion(constantBuffer, 0, 64, 0, 0, viewBuffer, 0, nullptr);
		mContext->CopySubresourceRegion(constantBuffer, 0, 128, 0, 0, projectionBuffer, 0, nullptr);

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
		static ID3D11Device* device = mDevice;
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

					if (mcp != nullptr)
					{
						reflect->AddBuffer(mcp);
					}
				}

				for (uint i = 0; i < resourceCount; i++)
				{
					MP* mp = reflect->ReflectTexture(device, i);

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

			for (auto& i : vertConsts) { mMaterial->AddBuffer(i.second); }
			for (auto& i : vertTextures) { mMaterial->AddTexture(i.second); }

		}
		if (geoReflect != nullptr)
		{
			for (auto& i : geoConsts) { mMaterial->AddBuffer(i.second); }
			for (auto& i : geoTextures) { mMaterial->AddTexture(i.second); }

		}
		if (domReflect != nullptr)
		{
			for (auto& i : domConsts) { mMaterial->AddBuffer(i.second); }
			for (auto& i : domTextures) { mMaterial->AddTexture(i.second); }

		}
		if (hulReflect != nullptr)
		{
			for (auto& i : hulConsts) { mMaterial->AddBuffer(i.second); }
			for (auto& i : hulTextures) { mMaterial->AddTexture(i.second); }

		}

		if (pixReflect != nullptr)
		{
			for (auto& i : pixConsts) { mMaterial->AddBuffer(i.second); }
			for (auto& i : pixTextures) { mMaterial->AddTexture(i.second); }

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

		stage_UpdateReservedResources(material, shader);
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