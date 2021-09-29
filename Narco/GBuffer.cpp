#include"inc/GBuffer.h"
#include"inc/Mesh.h"
#include"inc/Shader.h"
namespace NARCO
{
	GBuffer::GBuffer(ID3D11Device* device, unsigned int width, unsigned int height)
		: mBufferCount(ARRAYSIZE(GBufferFormats)), mDevice(device), mWidth(width), mHeight(height),
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
	
		mScreenQuadMesh = new Mesh(device, PRIMITIVE_QUAD);
		mScreenQuadShader = new Shader("built-in/hlsl/Deferred_FinalPass.hlsl", SHADER_VERTEX | SHADER_PIXEL);
		mScreenQuadShader->Compile(device);
	
	}
	GBuffer::~GBuffer()
	{
	}
	void GBuffer::DrawScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* backBuffer)
	{
		static ID3D11Buffer* vertex[] = { mScreenQuadMesh->GetVertex() };
		static ID3D11Buffer* index = mScreenQuadMesh->GetIndex();
		static ID3D11VertexShader* vs = mScreenQuadShader->GetVS();
		static ID3D11PixelShader* ps = mScreenQuadShader->GetPS();
		static ID3D11InputLayout* il = mScreenQuadShader->GetIL();
		static unsigned int strides[] = { sizeof(Vertex_Quad) };
		static unsigned int offsets[] = { 0 };
		context->VSSetShader(vs, nullptr, 0);
		context->PSSetShader(ps, nullptr, 0);

		context->IASetInputLayout(il);
		context->IASetVertexBuffers(0, 1, vertex, strides, offsets);
		context->IASetIndexBuffer(index, DXGI_FORMAT_R32_UINT, 0);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->OMSetRenderTargets(1, &backBuffer, nullptr);

		context->DrawIndexed(6, 0, 0);

		//context->ClearState();
	}
}