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
	void Renderer::stage_Vertex(const Material* material, const Shader* shader)
	{


	}
	void Renderer::stage_Geometry(const Material* material, const Shader* shader)
	{

	}
	void Renderer::stage_Domain(const Material* material, const Shader* shader)
	{
	}
	void Renderer::stage_Hull(const Material* material, const Shader* shader)
	{
	}
	void Renderer::stage_Pixel(const Material* material, const Shader* shader)
	{
	}
	void Renderer::start()
	{
		const Scene* scene = mRoot->GetScene();
		mContext = scene->GetContext();

		mTransform = mRoot->GetComponent<Transform>();
		mRenderCamera = scene->FindGameObjectWithTag("Main Camera")->GetComponent<Camera>();

		ID3D11Buffer* transformBuffer = mTransform->GetBuffer();
		ID3D11Buffer* viewBuffer = mRenderCamera->GetViewBuffer();
		ID3D11Buffer* projectionBuffer = mRenderCamera->GetProjectionBuffer();

		ID3D11Buffer* cameraBuffers[] = { viewBuffer, projectionBuffer };
		
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
			unsigned int pixCbCount = pixShaderDesc.ConstantBuffers;
			unsigned int pixTexCount = pixShaderDesc.BoundResources;

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

				auto lookup = textureProperties.find(MakeHash(pixTexDesc.Name));

				if (lookup == textureProperties.end())
				{
					continue;
				}

				MP* reg = lookup->second;
				mPixelTextures.emplace_back(reg->Register.Get());
			}

			if (pixCbCount > 0)
			{
				mContext->PSSetConstantBuffers(0, pixCbCount, mPixelConstantBuffers.data());
			}
			if (pixTexCount > 0)
			{
				mContext->PSSetShaderResources(0, pixTexCount - pixCbCount, mPixelTextures.data());
			}
		}

		



	}

	void Renderer::update(float delta)
	{
		const Material* material = mMaterial;
		const Shader* shader = mMaterial->GetShader();
		ID3D11VertexShader* vs = shader->GetVS();
		ID3D11GeometryShader* gs = shader->GetGS();
		ID3D11DomainShader* ds = shader->GetDS();
		ID3D11HullShader* hs = shader->GetHS();
		ID3D11PixelShader* ps = shader->GetPS();

		static ID3D11Buffer* vertex[] = { mMesh->GetVertex() };
		static ID3D11Buffer* index = mMesh->GetIndex();
		static unsigned int strides[] = { mMesh->GetStride() };
		static unsigned int offsets[] = { 0 };

		auto textureRegisters = material->GetInputTextureRegisters();
		auto constantRegisters = material->GetInputConstantRegisters();

		unsigned int vertexConstantBuffers = shader->GetVertexConstantBufferCount();
		unsigned int vertexBoundResources = shader->GetVertexBoundResourceCount();

		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mContext->IASetVertexBuffers(0, 1, vertex, strides, offsets);
		mContext->IASetIndexBuffer(index, DXGI_FORMAT_R32_UINT, 0);
		mContext->IASetInputLayout(shader->GetIL());

		// 프로퍼티 업데이트
		for (unsigned int i=0;i<)
		{
			
		}

		if (vs != nullptr)
		{
			mContext->VSSetShader(vs, nullptr, 0);
			mContext->VSSetConstantBuffers(0, mVertexConstantBuffers.size(), mVertexConstantBuffers.data());
			mContext->VSSetShaderResources(0, mVertexTextures.size(), mVertexTextures.data());

			//	stage_Vertex(material, shader);

		}

		if (gs != nullptr)
		{
			mContext->GSSetShader(gs, nullptr, 0);
			//	stage_Geometry(material, shader);
		}

		if (ds != nullptr)
		{
			mContext->DSSetShader(ds, nullptr, 0);
			//	stage_Domain(material, shader);
		}

		if (hs != nullptr)
		{
			mContext->HSSetShader(hs, nullptr, 0);
			//	stage_Hull(material, shader);
		}

		if (ps != nullptr)
		{
			mContext->PSSetShader(ps, nullptr, 0);
			//	stage_Pixel(material, shader);
			mContext->PSSetConstantBuffers(0, mPixelConstantBuffers.size(), mPixelConstantBuffers.data());
			
			
			mContext->PSSetShaderResources(0, mPixelTextures.size(), mPixelTextures.data());

		}

		mContext->DrawIndexed(mMesh->GetIndexCount(), 0, 0);


	}

	void Renderer::render(float delta)
	{
	}

	void Renderer::release()
	{
	}
}