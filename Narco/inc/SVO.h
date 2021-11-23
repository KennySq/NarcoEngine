#pragma once
#include"Common.h"
#include"MeshLoader.h"
#include"Stage.h"

#include"Camera.h"

#include"Octree.h"

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

		void VisualizeTexture();
		ID3D11ShaderResourceView* GetDebugTexture() const { return mDebugTextureSRV.Get(); }
	private:
		Mesh* mMesh;
		Camera* mCamera;

		ID3D11Device* const mDevice;

		Stage<ID3D11ComputeShader>* mInitCS;
		Stage<ID3D11ComputeShader>* mComputeCS;

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


		ComPtr<ID3D11Buffer> mDebugVertexBuffer;
		ComPtr<ID3D11Buffer> mDebugIndexBuffer;
		Material* mDebugMaterial;
		ComPtr<ID3D11ShaderResourceView> mDebugShaderResourceView;


		struct Edge
		{
			unsigned int A;
			unsigned int B;
		};

		ComPtr<ID3D11Buffer> mEdgeBuffer;
		ComPtr<ID3D11UnorderedAccessView> mEdgeBufferUAV;

		ID3D11DeviceContext* const mContext;

		Octree* mOctree;
		
		
	};
}