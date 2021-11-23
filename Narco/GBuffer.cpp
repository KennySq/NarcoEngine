#include"inc/GBuffer.h"
#include"inc/Mesh.h"
#include"inc/Material.h"

namespace NARCO
{
	GBuffer::GBuffer(ID3D11Device* device, uint width, uint height)
		: mBufferCount(GBufferFormats.size()), mDevice(device), mWidth(width), mHeight(height),
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

		mScreenQuadShader = new Material("built-in/hlsl/Deferred_FinalPass.hlsl", STAGE_VERTEX | STAGE_PIXEL);
		
		D3D11_SAMPLER_DESC samplerDirectDesc{};

		samplerDirectDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDirectDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDirectDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDirectDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDirectDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDirectDesc.MaxLOD = 1;

		HRESULT result = mDevice->CreateSamplerState(&samplerDirectDesc, mSamplerStateDirect.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create sampler state direct.");
			return;
		}
	}
	GBuffer::~GBuffer()
	{
	}
	void GBuffer::DrawScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* backBuffer, ID3D11ShaderResourceView* debugSrv)
	{
		static ID3D11Buffer* vertex[] = { mScreenQuadMesh->GetVertex() };
		static ID3D11Buffer* index = mScreenQuadMesh->GetIndex();

		static Stage<ID3D11VertexShader>* vertexStage = mScreenQuadShader->GetVertex();
		static Stage<ID3D11PixelShader>* pixelStage = mScreenQuadShader->GetPixel();

		static ID3D11SamplerState* samplerStates[] = { mSamplerStateDirect.Get() };
		static ID3D11SamplerState* nullSampler[] = { nullptr };
	
		static unsigned int strides[] = { sizeof(Vertex_Quad) };
		static unsigned int offsets[] = { 0 };
		
		static ID3D11ShaderResourceView* nullSrv[6] = { nullptr };
		static ID3D11RenderTargetView* nullRtv[] = { nullptr };

		static std::vector<ID3D11ShaderResourceView*> bufferSRV;
		static std::vector<ID3D11ShaderResourceView*> lightSRV;

		//context->OMSetRenderTargets(1, null, nullptr);


		uint startIndex = 0;

		/*for (unsigned int i = 0; i < mBufferCount; i++)
		{
			bufferSRV.push_back(mBuffers[i]->GetShaderResource());
		}*/

		bufferSRV.push_back(debugSrv);

		context->VSSetShader(vertexStage->GetShader(), nullptr, 0);
		context->PSSetShader(pixelStage->GetShader(), nullptr, 0);
		context->PSSetSamplers(0, 1, samplerStates);

		context->IASetInputLayout(mScreenQuadShader->GetInputLayout());
		context->IASetVertexBuffers(0, 1, vertex, strides, offsets);
		context->IASetIndexBuffer(index, DXGI_FORMAT_R32_UINT, 0);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//context->PSSetShaderResources(0, mBufferCount, bufferSRV.data());
		context->PSSetShaderResources(0, 1, bufferSRV.data());

		context->OMSetRenderTargets(1, &backBuffer, nullptr);


		context->DrawIndexed(6, 0, 0);

		context->PSSetShaderResources(0, 1, nullSrv);
	//	context->OMSetRenderTargets(1, nullRtv, nullptr);
		context->PSSetSamplers(0, 1, nullSampler);
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