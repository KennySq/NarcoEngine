#pragma once
#include"Common.h"
#include"MeshLoader.h"
#include"Stage.h"

#include"Camera.h"

namespace NARCO
{
	class SVO
	{
		struct Voxel
		{
			unsigned int TriangleIndex;


		};

	public:
		SVO(ID3D11Device* device, ID3D11DeviceContext* context, Mesh* mesh, Camera* camera);

		void Load();
		void Compute();

	private:
		Mesh* mMesh;
		Camera* mCamera;

		ID3D11Device* const mDevice;

		Stage<ID3D11ComputeShader>* mCompute;
		
		ComPtr<ID3D11Texture2D> mTriangleTexture;
		ComPtr<ID3D11ShaderResourceView> mTriangleTextureSRV;

		ComPtr<ID3D11Buffer> mTriangleBuffer;
		ComPtr<ID3D11ShaderResourceView> mTriangleBufferSRV;

		ComPtr<ID3D11Buffer> mVertexBuffer;
		ComPtr<ID3D11ShaderResourceView> mVertexBufferSRV;

		ComPtr<ID3D11Buffer> mConstantBuffer;

		ComPtr<ID3D11Texture2D> mDebugTexture;
		ComPtr<ID3D11UnorderedAccessView> mDebugTextureUAV;
		ComPtr<ID3D11ShaderResourceView> mDebugTextureSRV;


		ID3D11DeviceContext* const mContext;
		
		
	};
}