#include"inc/Renderer.h"
#include"inc/Scene.h"
namespace NARCO
{
	struct stageInfo
	{
		uint bufferCount;
		uint shaderResourceCount;
		uint samplerCount;

		std::vector<ID3D11Buffer*> buffers;
		std::vector<ID3D11ShaderResourceView*> shaderResources;
		std::vector<ID3D11SamplerState*> samplerStates;
	};

	struct bindStage
	{
		template<typename _Ty>
		stageInfo operator() (Stage<_Ty>* stage) const
		{
			stageInfo info;

			info.bufferCount = stage->GetBufferCount();
			info.shaderResourceCount = stage->GetShaderResourceCount();
			info.samplerCount = stage->GetSamplerStateCount();

			auto stageBuffers = stage->GetBufferPointer();
			auto stageSrv = stage->GetShaderResources();
			auto stageSamplers = stage->GetSamplerStates();

			for (uint i = 0; i < info.bufferCount; i++)
			{
				info.buffers.emplace_back(stageBuffers[i]->Resource.Get());
			}

			for (uint i = 0; i < info.shaderResourceCount; i++)
			{
				info.shaderResources.emplace_back(stageSrv[i]->Resource.Get());

			}

			for (uint i = 0; i < info.samplerCount; i++)
			{
				info.samplerStates.emplace_back(stageSamplers[i]->Resource.Get());
			}

			return info;
		}
	};

	Renderer::Renderer()
	{
	}
	Renderer::~Renderer()
	{
	}

	void Renderer::awake()
	{
		mRenderScene = mRoot->GetScene();
	
		GameObject* cameraObject = mRenderScene->FindGameObjectWithTag("Main Camera");
		mRenderCamera = cameraObject->GetComponent<Camera>();
	
		mRenderTransform = mRoot->GetComponent<Transform>();
	}
	void Renderer::start()
	{
	}

	void Renderer::update(float delta)
	{
		static ID3D11DeviceContext* context = mContext;
		static ID3D11ShaderResourceView* nullSRV[] = { nullptr };
		static const char* cbufferName = "Constants";
		static const char* variableName = "gWorld";


		auto lamdaBindStage = bindStage{};

		uint passCount = mMaterials.size();

		for (uint i = 0; i < passCount; i++)
		{
			Material* material = mMaterials[i];

			XMMATRIX loadMat = XMMatrixTranspose(mRenderTransform->GetMatrix());
			XMMATRIX viewMat = mRenderCamera->GetView();
			XMMATRIX projMat = mRenderCamera->GetProjection();

			struct InstanceData
			{
				XMMATRIX World;
				XMMATRIX View;
				XMMATRIX Projection;
			};

			InstanceData instData = { loadMat, viewMat, projMat };

			material->MapConstantBuffer(cbufferName, variableName, RESOURCE_CBUFFER, &instData, sizeof(instData));

			mRenderCamera->RenderMaterial = material;
			mRenderTransform->RenderMaterial = material;

			Stage<ID3D11VertexShader>* vertex = material->GetVertex();
			Stage<ID3D11GeometryShader>* geometry = material->GetGeometry();
			Stage<ID3D11DomainShader>* domain = material->GetDomain();
			Stage<ID3D11HullShader>* hull = material->GetHull();
			Stage<ID3D11PixelShader>* pixel = material->GetPixel();

			if (mMesh == nullptr)
			{
				Debug::Log("no mesh found. " + mName);
				return;
			}

			uint indexCount = mMesh->GetIndexCount();
			uint strides[] = { mMesh->GetStride() };
			uint offsets[] = { 0 };
			ID3D11Buffer* vertexBuffer[] = { mMesh->GetVertex() };
			ID3D11Buffer* indexBuffer = mMesh->GetIndex();
			ID3D11InputLayout* inputLayout = material->GetInputLayout();

			context->IASetVertexBuffers(0, 1, vertexBuffer, strides, offsets);
			context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			context->IASetInputLayout(inputLayout);
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			if (vertex != nullptr)
			{
				stageInfo info = lamdaBindStage(vertex);

				context->VSSetShader(vertex->GetShader(), nullptr, 0);

				if (info.bufferCount > 0)
				{
					context->VSSetConstantBuffers(0, info.bufferCount, info.buffers.data());
				}

				if (info.shaderResourceCount > 0)
				{
					context->VSSetShaderResources(0, info.shaderResourceCount, info.shaderResources.data());
				}

				if (info.samplerCount > 0)
				{
					context->VSSetSamplers(0, info.samplerCount, info.samplerStates.data());
				}

			}

			if (geometry != nullptr)
			{
				auto info = lamdaBindStage(geometry);

				context->GSSetShader(geometry->GetShader(), nullptr, 0);

				if (info.bufferCount > 0)
				{
					context->GSSetConstantBuffers(0, info.bufferCount, info.buffers.data());
				}

				if (info.shaderResourceCount > 0)
				{
					context->GSSetShaderResources(0, info.shaderResourceCount, info.shaderResources.data());
				}

				if (info.samplerCount > 0)
				{
					context->GSSetSamplers(0, info.samplerCount, info.samplerStates.data());
				}
			}

			if (domain != nullptr)
			{
				auto info = lamdaBindStage(domain);

				context->DSSetShader(domain->GetShader(), nullptr, 0);

				if (info.bufferCount > 0)
				{
					context->DSSetConstantBuffers(0, info.bufferCount, info.buffers.data());
				}

				if (info.shaderResourceCount > 0)
				{
					context->DSSetShaderResources(0, info.shaderResourceCount, info.shaderResources.data());
				}

				if (info.samplerCount > 0)
				{
					context->DSSetSamplers(0, info.samplerCount, info.samplerStates.data());
				}
			}

			if (hull != nullptr)
			{
				auto info = lamdaBindStage(hull);

				context->HSSetShader(hull->GetShader(), nullptr, 0);

				if (info.bufferCount > 0)
				{
					context->HSSetConstantBuffers(0, info.bufferCount, info.buffers.data());
				}

				if (info.shaderResourceCount > 0)
				{
					context->HSSetShaderResources(0, info.shaderResourceCount, info.shaderResources.data());
				}

				if (info.samplerCount > 0)
				{
					context->HSSetSamplers(0, info.samplerCount, info.samplerStates.data());
				}
			}

			if (pixel != nullptr)
			{
				auto info = lamdaBindStage(pixel);

				context->PSSetShader(pixel->GetShader(), nullptr, 0);

				if (info.bufferCount > 0)
				{
					context->PSSetConstantBuffers(0, info.bufferCount, info.buffers.data());
				}

				if (info.shaderResourceCount > 0)
				{
					context->PSSetShaderResources(0, info.shaderResourceCount, info.shaderResources.data());
				}

				if (info.samplerCount > 0)
				{
					context->PSSetSamplers(0, info.samplerCount, info.samplerStates.data());
				}
			}

			context->DrawIndexed(indexCount, 0, 0);

			context->VSSetShaderResources(0, 1, nullSRV);
			context->GSSetShaderResources(0, 1, nullSRV);
			context->DSSetShaderResources(0, 1, nullSRV);
			context->HSSetShaderResources(0, 1, nullSRV);
			context->PSSetShaderResources(0, 1, nullSRV);
		}
	}
	void Renderer::render(float delta)
	{

	}
	void Renderer::release()
	{
	}
}