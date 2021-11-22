#include"../inc/SVO.h"

namespace NARCO
{
	SVO::SVO(ID3D11Device* device, ID3D11DeviceContext* context, Mesh* mesh, Camera* camera)
		: mDevice(device), mContext(context), mMesh(mesh), mCamera(camera)
	{
		
	}
	void SVO::Load()
	{
	//	mInitCS = new Stage<ID3D11ComputeShader>("built-in/hlsl/InitSVO.hlsl");
		mComputeCS = new Stage<ID3D11ComputeShader>("built-in/hlsl/ComputeSVO.hlsl");
		D3D11_MAPPED_SUBRESOURCE mapped{};
		ID3D11Buffer* indexBuffer = mMesh->GetIndex();
		ID3D11Buffer* vertexBuffer = mMesh->GetVertex();
		
		D3D11_BUFFER_DESC triangleBufferDesc{};
		D3D11_BUFFER_DESC vertexBufferDesc{};
		D3D11_BUFFER_DESC constantBufferDesc{};

		D3D11_SHADER_RESOURCE_VIEW_DESC vertexBufferSRVDesc{};
		D3D11_SHADER_RESOURCE_VIEW_DESC triangleBufferSRVDesc{};

		indexBuffer->GetDesc(&triangleBufferDesc);
		vertexBuffer->GetDesc(&vertexBufferDesc);

		triangleBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		triangleBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		
		vertexBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		vertexBufferDesc.StructureByteStride = sizeof(Vertex_Static);
		vertexBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		triangleBufferSRVDesc.Format = DXGI_FORMAT_R32_UINT;
		triangleBufferSRVDesc.Buffer.ElementWidth = sizeof(unsigned int);
		triangleBufferSRVDesc.Buffer.NumElements = triangleBufferDesc.ByteWidth / sizeof(unsigned int);
		triangleBufferSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

		vertexBufferSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
		vertexBufferSRVDesc.Buffer.ElementWidth = sizeof(Vertex_Static);
		vertexBufferSRVDesc.Buffer.NumElements = vertexBufferDesc.ByteWidth / sizeof(Vertex_Static);
		vertexBufferSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

		HRESULT result = mDevice->CreateBuffer(&triangleBufferDesc, nullptr, mTriangleBuffer.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create triangle buffer.");
			return;
		}

		result = mDevice->CreateShaderResourceView(mTriangleBuffer.Get(), &triangleBufferSRVDesc, mTriangleBufferSRV.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create triangle buffer shader resource view.");
			return;
		}

		result = mDevice->CreateBuffer(&vertexBufferDesc, nullptr, mVertexBuffer.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create vertexx buffer.");
			return;
		}

		result = mDevice->CreateShaderResourceView(mVertexBuffer.Get(), &vertexBufferSRVDesc, mVertexBufferSRV.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create vertexx buffer shader resource view");
			return;
		}


		mContext->CopyResource(mTriangleBuffer.Get(), indexBuffer);
		mContext->CopyResource(mVertexBuffer.Get(), vertexBuffer);

		mContext->Map(mTriangleBuffer.Get(), 0, D3D11_MAP_READ, 0, &mapped);

		unsigned int* triangleList = reinterpret_cast<unsigned int*>(mapped.pData);
		unsigned int triangleCount = mMesh->GetIndexCount();
		mContext->Unmap(mTriangleBuffer.Get(), 0);

		mContext->Map(mVertexBuffer.Get(), 0, D3D11_MAP_READ, 0, &mapped);

		Vertex_Static* vertices = reinterpret_cast<Vertex_Static*>(mapped.pData);
		unsigned int vertexCount = vertexBufferDesc.ByteWidth / sizeof(Vertex_Static);

		float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
		float maxX = FLT_MIN, maxY = FLT_MIN, maxZ = FLT_MIN;

		for (unsigned int i = 0; i < vertexCount; i++)
		{
			float x = vertices[i].mPosition.x;
			float y = vertices[i].mPosition.y;
			float z = vertices[i].mPosition.z;

			if (x < minX)
			{
				minX = x;
			}

			if (y < minY)
			{
				minY = y;
			}

			if (z < minZ)
			{
				minZ = z;
			}

			if (x > maxX)
			{
				maxX = x;
			}

			if (y > maxY)
			{
				maxY = y;
			}

			if (z > maxZ)
			{
				maxZ = z;
			}
		}

		XMFLOAT3 boundMin = { minX, minY, minZ };
		XMFLOAT3 boundMax = { maxX, maxY, maxZ };

		AABB rootBound(boundMin, boundMax);
		mOctree = new Octree(mDevice, mContext, rootBound);

		mContext->Unmap(mVertexBuffer.Get(), 0);

		std::vector<unsigned int> triangleVector;
		std::vector<Vertex_Static> vertexVector;

		for (unsigned int i = 0; i < triangleCount; i++)
		{
			triangleVector.push_back(triangleList[i]);
		}

		for (unsigned int i = 0; i < vertexCount; i++)
		{
			vertexVector.push_back(vertices[i]);
		}

		D3D11_TEXTURE2D_DESC debugTextureDesc{};
		debugTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		debugTextureDesc.MipLevels = 1;
		debugTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		debugTextureDesc.Width = 1280;
		debugTextureDesc.Height = 720;
		debugTextureDesc.ArraySize = 1;
		debugTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		debugTextureDesc.SampleDesc.Count = 1;

		D3D11_SHADER_RESOURCE_VIEW_DESC debugTextureSRVDesc{};

		debugTextureSRVDesc.Format = debugTextureDesc.Format;
		debugTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		debugTextureSRVDesc.Texture2D.MipLevels = 1;

		D3D11_UNORDERED_ACCESS_VIEW_DESC debugTextureUAVDesc{};

		debugTextureUAVDesc.Format = debugTextureDesc.Format;
		debugTextureUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

		result = mDevice->CreateTexture2D(&debugTextureDesc, nullptr, mDebugTexture.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create debug texture.");
			return;
		}

		result = mDevice->CreateShaderResourceView(mDebugTexture.Get(), &debugTextureSRVDesc, mDebugTextureSRV.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create debug texture shader resource view.");
			return;
		}

		result = mDevice->CreateUnorderedAccessView(mDebugTexture.Get(), &debugTextureUAVDesc, mDebugTextureUAV.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create debug texture unordered access view.");
			return;
		}

		{
			struct SVOConstant
			{
				XMMATRIX view;
				XMMATRIX projection;
				unsigned int currentDepth;
				unsigned int maxDepth;
			};

			constantBufferDesc.ByteWidth = sizeof(SVOConstant);
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			
			//XMMATRIX view = mCamera->GetView();
			//XMMATRIX projection = mCamera->GetProjection();
			//XMVECTOR viewPosition = mCamera->GetPosition();
			//XMVECTOR viewDirection = mCamera->GetDirection();

			XMMATRIX view{};// = mCamera->GetView();
			XMMATRIX projection{};// = mCamera->GetProjection();
			XMVECTOR viewPosition = XMVectorSet(-1, 0, 0, 0);// = mCamera->GetPosition();
			XMVECTOR viewDirection = XMVectorSet(0, 0, 0, 0);// = mCamera->GetDirection();


			SVOConstant constant = { view, projection, 0, 8 };

			D3D11_SUBRESOURCE_DATA constantSubresource{};

			constantSubresource.pSysMem = reinterpret_cast<const void*>(&constant);

			result = mDevice->CreateBuffer(&constantBufferDesc, &constantSubresource, mConstantBuffer.GetAddressOf());
			if (result != S_OK)
			{
				Debug::Log("failed to create constant buffer.");
				return;
			}

			//
			D3D11_BUFFER_DESC edgeBufferDesc{};
			D3D11_UNORDERED_ACCESS_VIEW_DESC edgeBufferUAVDesc{};

			edgeBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
			edgeBufferDesc.ByteWidth = sizeof(Edge) * 3 * triangleCount;
			edgeBufferDesc.StructureByteStride = sizeof(Edge);
			
			edgeBufferUAVDesc.Buffer.NumElements = 3 * triangleCount;
			edgeBufferUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
			edgeBufferUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

			result = mDevice->CreateBuffer(&edgeBufferDesc, nullptr, mEdgeBuffer.GetAddressOf());
			if (result != S_OK)
			{
				Debug::Log("failed to create edge buffer.");
				return;
			}

			//result = mDevice->CreateUnorderedAccessView(mEdgeBuffer.Get(), &edgeBufferUAVDesc, mEdgeBufferUAV.GetAddressOf());
			//if (result != S_OK)
			//{
			//	Debug::Log("failed to create edge buffer unordered access view.");
			//	return;
			//}

		//	ID3D11Buffer* constantBuffers[] = { mConstantBuffer.Get() };
		//	ID3D11ShaderResourceView* shaderResources[] = { mVertexBufferSRV.Get(), mTriangleBufferSRV.Get() };
		//	ID3D11UnorderedAccessView* unorderedAccesses[] = { mDebugTextureUAV.Get(), mEdgeBufferUAV.Get() };

		//	mContext->CSSetShader(mComputeCS->GetShader(), nullptr, 0);
		//	mContext->CSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
		//	mContext->CSSetShaderResources(0, ARRAYSIZE(shaderResources), shaderResources);
		//	mContext->CSSetUnorderedAccessViews(0, ARRAYSIZE(unorderedAccesses), unorderedAccesses, nullptr);

		////	mContext->Dispatch(1280 / 32, 720 / 30, 1);
		}
	}
	void SVO::Compute()
	{
		ID3D11Buffer* constantBuffers[] = { mConstantBuffer.Get() };
		ID3D11ShaderResourceView* shaderResources[] = { mVertexBufferSRV.Get(), mTriangleBufferSRV.Get() };
		ID3D11UnorderedAccessView* unorderedAccesses[] = { mDebugTextureUAV.Get(), mOctree->GetTextureUAV() };

		mContext->CSSetShader(mComputeCS->GetShader(), nullptr, 0);
		mContext->CSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
		mContext->CSSetShaderResources(0, ARRAYSIZE(shaderResources), shaderResources);
		mContext->CSSetUnorderedAccessViews(0, ARRAYSIZE(unorderedAccesses), unorderedAccesses, nullptr);
		
		mContext->Dispatch(32, 32, 32);
	}
}