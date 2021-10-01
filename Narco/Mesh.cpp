#include"inc/Mesh.h"

namespace NARCO
{
	Mesh::Mesh()
	{
	}
	Mesh::Mesh(ID3D11Device* device, eMeshPrimitive primitiveTag)
	{
		if (primitiveTag == PRIMITIVE_QUAD)
		{
			XMFLOAT3 position[] =
			{
				{-1,1, 0},
				{1,1,0},
				{-1,-1,0},
				{1,-1,0}
			};

			XMFLOAT2 uv[] =
			{
				{0,0},
				{1,0},
				{0,1},
				{1,1}
			};

			Vertex_Quad vertices[] =
			{
				{position[0], uv[0]},
				{position[1], uv[1]},
				{position[2], uv[2]},
				{position[3], uv[3]},
			};

			unsigned int indices[] =
			{
				0,1,2,1,3,2
			};

			D3D11_BUFFER_DESC vDesc{}, iDesc{};
			D3D11_SUBRESOURCE_DATA subData{};

			vDesc.ByteWidth = sizeof(Vertex_Quad) * 4;
			vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			
			iDesc.ByteWidth = sizeof(unsigned int) * 6;
			iDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			subData.pSysMem = vertices;

			HRESULT result = device->CreateBuffer(&vDesc, &subData, mVertex.GetAddressOf());
			if (result != S_OK)
			{
				ExceptionError(E_INVALIDARG, "invalid argument.");
				return;
			}

			subData.pSysMem = indices;

			result = device->CreateBuffer(&iDesc, &subData, mIndex.GetAddressOf());
			if (result != S_OK)
			{
				ExceptionError(E_INVALIDARG, "invalid argument.");
				return;
			}

			return;


		}

	}

}