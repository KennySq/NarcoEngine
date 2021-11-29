#pragma once
#include"Common.h"

#include"D3DTexture2D.h"
#include"D3DDepthStencil.h"

namespace NARCO
{
	static DXGI_FORMAT GBufferFormats[] =
	{
		DXGI_FORMAT_R32G32B32A32_FLOAT, // Position
		DXGI_FORMAT_R32G32B32A32_FLOAT, // WorldPosition
		DXGI_FORMAT_R32G32B32A32_FLOAT, // Normal
		DXGI_FORMAT_R32G32B32A32_FLOAT, // WorldNormal
		DXGI_FORMAT_R32G32_FLOAT,		// UV
		DXGI_FORMAT_R32G32B32A32_FLOAT, // Albedo
	};

	static constexpr uint GBufferSize = ARRAYSIZE(GBufferFormats);

	class Mesh;
	class Material;
	class GBuffer
	{
	public:
		GBuffer(ID3D11Device* device, uint width, uint height);
		~GBuffer();

		ID3D11RenderTargetView* const * GetRenderTargets() { return mRenderTargets.data(); }
		ID3D11DepthStencilView* GetDepthStencil() { return mDepth->GetDepthStencilView(); }

		const unsigned int GetBufferCount() const { return mBufferCount; }

		void DrawScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* backBuffer);
		void ClearBuffer(ID3D11DeviceContext* context, const float* clearColors);

		void Bound(ID3D11DeviceContext* context) const {
			context->OMSetRenderTargets(mBufferCount, mRenderTargets.data(), mDepth->GetDepthStencilView());
		}

		void Unbound(ID3D11DeviceContext* context) const
		{
			static ID3D11RenderTargetView* nullRTV[] = { nullptr };
			context->OMSetRenderTargets(1, nullRTV, nullptr);
		}

	private:
		ID3D11Device* mDevice;

		std::vector<D3DTexture2D*> mBuffers;
		D3DDepthStencil* mDepth;

		std::vector<ID3D11RenderTargetView*> mRenderTargets;
		std::vector<ID3D11ShaderResourceView*> mShaderResources;

		const unsigned int mBufferCount;
		
		uint mWidth;
		uint mHeight;

		Mesh* mScreenQuadMesh;
		Material* mScreenQuadShader;
	};
}