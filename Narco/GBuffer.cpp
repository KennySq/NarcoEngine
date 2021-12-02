#include"inc/GBuffer.h"
#include"inc/Mesh.h"
#include"inc/Material.h"

namespace NARCO
{
	GBuffer::GBuffer(ID3D11Device* device, uint width, uint height)
		: mBufferCount(GBufferSize), mDevice(device), mWidth(width), mHeight(height),
		mRenderTargets(8), mShaderResources(128)
	{
		

		for (unsigned int i = 0; i < mBufferCount; i++)
		{
			D3DTexture2D* buffer = new D3DTexture2D(device, GBufferFormats[i],
				D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
				D3D11_USAGE_DEFAULT, width, height, 1, 0);
			mBuffers.emplace_back(buffer);
			
			mRenderTargets[i] = buffer->GetRenderTarget();
			mShaderResources[i] = buffer->GetShaderResource();
		}

		mDepth = new D3DDepthStencil(device, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT, width, height, 1);
	
		mScreenQuadMesh = new Mesh("SCREEN_QUAD", device, PRIMITIVE_QUAD);

		mScreenQuadShader = new Material("built-in/hlsl/Deferred_FinalPass.hlsl", STAGE_VERTEX | STAGE_PIXEL, true);
		
	
	}
	GBuffer::~GBuffer()
	{
	}
	void GBuffer::DrawScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* backBuffer)
	{
		static LightHandler* lightHandler = LightHandler::GetInstance();

		ID3D11Buffer* directionalLights = lightHandler->GetDirectionalLights();
		ID3D11Buffer* pointLights = lightHandler->GetPointLights();

		ID3D11Buffer* lightBuffers[] = { directionalLights, pointLights };

		static ID3D11Buffer* vertex[] = { mScreenQuadMesh->GetVertex() };
		static ID3D11Buffer* index = mScreenQuadMesh->GetIndex();

		static Stage<ID3D11VertexShader>* vertexStage = mScreenQuadShader->GetVertex();
		static Stage<ID3D11PixelShader>* pixelStage = mScreenQuadShader->GetPixel();

		static unsigned int strides[] = { sizeof(Vertex_Quad) };
		static unsigned int offsets[] = { 0 };
		
		static ID3D11ShaderResourceView* nullSrv[GBufferSize] = { nullptr };
		static ID3D11RenderTargetView* nullRtv[] = { nullptr };

		static std::vector<ID3D11ShaderResourceView*> bufferSRV;
		static std::vector<ID3D11ShaderResourceView*> lightSRV;

		uint startIndex = 0;

		for (unsigned int i = 0; i < mBufferCount; i++)
		{
			bufferSRV.push_back(mBuffers[i]->GetShaderResource());
		}

		ID3D11ClassInstance** classInstances = pixelStage->GetClassInstances()->Instances;
		uint classInstanceCount = pixelStage->GetClassInstances()->mInstanceCount;
		SharedResource<ID3D11Buffer>* const* buffers = pixelStage->GetBufferPointer();
		
		std::vector<ID3D11Buffer*> psBuffer;

		uint psBufferCount = pixelStage->GetBufferCount();

		for (unsigned int i = 0; i < psBufferCount; i++)
		{
			psBuffer.emplace_back(buffers[i]->Resource.Get());
		}

		context->VSSetShader(vertexStage->GetShader(), nullptr, 0);
		context->PSSetShader(pixelStage->GetShader(), classInstances, classInstanceCount);

		context->IASetInputLayout(mScreenQuadShader->GetInputLayout());
		context->IASetVertexBuffers(0, 1, vertex, strides, offsets);
		context->IASetIndexBuffer(index, DXGI_FORMAT_R32_UINT, 0);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->PSSetConstantBuffers(0, pixelStage->GetBufferCount(), psBuffer.data());
		context->PSSetShaderResources(0, mBufferCount, bufferSRV.data());

		context->OMSetRenderTargets(1, &backBuffer, nullptr);

		context->DrawIndexed(6, 0, 0);

		context->PSSetShaderResources(0, 1, nullSrv);

		bufferSRV.clear();
	}
	void GBuffer::ClearBuffer(ID3D11DeviceContext* context, const float* clearColors)
	{
		for (unsigned int i = 0; i < mBufferCount; i++)
		{
			context->ClearRenderTargetView(mBuffers[i]->GetRenderTarget(), clearColors);
		}
		context->ClearDepthStencilView(mDepth->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);


	}
}