#pragma once
#include"Common.h"

#include"D3DTexture2D.h"
#include"D3DDepthStencil.h"

namespace NARCO
{
	static std::vector<DXGI_FORMAT> GBufferFormats =
	{
		DXGI_FORMAT_R32G32B32A32_FLOAT, // Position
		DXGI_FORMAT_R32G32B32A32_FLOAT, // WorldPosition
		DXGI_FORMAT_R32G32B32A32_FLOAT, // Normal
		DXGI_FORMAT_R32G32B32A32_FLOAT, // WorldNormal
		DXGI_FORMAT_R32G32_FLOAT,		// UV
		DXGI_FORMAT_R32G32B32A32_FLOAT, // Albedo
	};
	class Mesh;
	class Shader;
	class GBuffer
	{
	public:
		GBuffer(ID3D11Device* device, unsigned int width, unsigned int height);
		~GBuffer();

		ID3D11RenderTargetView* const * GetRenderTargets() { return mRenderTargets.data(); }
		ID3D11DepthStencilView* GetDepthStencil() { return mDepth->GetDepthStencilView(); }

		const unsigned int GetBufferCount() const { return mBufferCount; }

		void DrawScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* backBuffer);
		void ClearBuffer(ID3D11DeviceContext* context);
	private:
		ID3D11Device* mDevice;

		std::vector<D3DTexture2D*> mBuffers;
		D3DDepthStencil* mDepth;

		std::vector<ID3D11RenderTargetView*> mRenderTargets;
		std::vector<ID3D11ShaderResourceView*> mShaderResources;

		const unsigned int mBufferCount;
		
		unsigned int mWidth;
		unsigned int mHeight;

		Mesh* mScreenQuadMesh;
		Shader* mScreenQuadShader;
	};
}