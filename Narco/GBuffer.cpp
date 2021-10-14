#include"inc/GBuffer.h"
#include"inc/Mesh.h"
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

		//mScreenQuadShader = new Shader("built-in/hlsl/Deferred_FinalPass.hlsl", SHADER_VERTEX | SHADER_PIXEL);
		//mScreenQuadShader->Compile(device);
	
	}
	GBuffer::~GBuffer()
	{
	}
	void GBuffer::DrawScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* backBuffer)
	{
		//static ID3D11Buffer* vertex[] = { mScreenQuadMesh->GetVertex() };
		//static ID3D11Buffer* index = mScreenQuadMesh->GetIndex();
		//static ID3D11VertexShader* vs = mScreenQuadShader->GetVS();
		//static ID3D11PixelShader* ps = mScreenQuadShader->GetPS();
		//static ID3D11InputLayout* il = mScreenQuadShader->GetIL();
		//static unsigned int strides[] = { sizeof(Vertex_Quad) };
		//static unsigned int offsets[] = { 0 };
		//static ID3D11ShaderResourceView* nullSrv[6] = { nullptr };
		//static ID3D11RenderTargetView* nullRtv[] = { nullptr };
		//static std::vector<ID3D11ShaderResourceView*> bufferSRV;
		//static std::vector<ID3D11ShaderResourceView*> lightSRV;

		////context->OMSetRenderTargets(1, null, nullptr);


		//uint startIndex = 0;

		//for (unsigned int i = 0; i < mBufferCount; i++)
		//{
		//	bufferSRV.push_back(mBuffers[i]->GetShaderResource());
		//}

		//context->VSSetShader(vs, nullptr, 0);
		//context->PSSetShader(ps, nullptr, 0);

		//context->IASetInputLayout(il);
		//context->IASetVertexBuffers(0, 1, vertex, strides, offsets);
		//context->IASetIndexBuffer(index, DXGI_FORMAT_R32_UINT, 0);

		//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//context->PSSetShaderResources(0, mBufferCount, bufferSRV.data());

		//context->OMSetRenderTargets(1, &backBuffer, nullptr);

		//context->DrawIndexed(6, 0, 0);

		//context->PSSetShaderResources(0, 1, nullSrv);
		//bufferSRV.clear();
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