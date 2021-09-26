#include "inc/MeshLoader.h"
namespace NARCO
{
	void MeshLoader::SetPath(const char* path)
	{
		mPath = path;

		std::string extend = mPath.substr(mPath.find_last_of("."));
		
		if (extend == ".fbx")
		{
			mMeshType = eMeshType::MESH_FBX;
		}

		if (extend == ".obj")
		{
			mMeshType = eMeshType::MESH_OBJ;
		}
	}

	void MeshLoader::Load()
	{
		if (mMeshType == eMeshType::MESH_NONE)
		{
			ExceptionWarning(E_INVALIDARG, "MeshLoader path was invalid or haven't been set yet.");
			return;
		}

		if (mMeshType == eMeshType::MESH_FBX)
		{
			bool result = fbx_loadFbx();
			if (result == false)
			{
				return;
			}

		}
	}
	void MeshLoader::Reload()
	{
	}

	void MeshLoader::Release()
	{
	}

	bool MeshLoader::fbx_loadFbx()
	{
		FbxManager* manager = FbxManager::Create();
		FbxIOSettings* io = FbxIOSettings::Create(manager, IOSROOT);

		manager->SetIOSettings(io);

		FbxImporter* importer = FbxImporter::Create(manager, "");

		const char* path = mPath.c_str();

		bool result = importer->Initialize(path, -1, manager->GetIOSettings());

		if (result == false)
		{
			auto errorStatus = importer->GetStatus();
			ExceptionWarning(E_FAIL, "FbxImporter initialization failed.");
			throw std::invalid_argument(errorStatus.GetErrorString());
		}

		FbxScene* scene = FbxScene::Create(manager, "Scene");
		importer->Import(scene);

		FbxNode* rootNode = scene->GetRootNode();

		fbx_loadNode(rootNode);
		


		// misc codes
		//FbxAxisSystem sceneAxis = scene->GetGlobalSettings().GetAxisSystem();
		//FbxAxisSystem::MayaYUp.ConvertScene(scene);
		//FbxGeometryConverter geometryConverter(manager);
		//geometryConverter.Triangulate(scene, true);







		return true;
	}
	void MeshLoader::fbx_loadNode(FbxNode* node)
	{
		FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

		if (nodeAttribute != nullptr)
		{
			if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				FbxMesh* mesh = node->GetMesh();
				fbx_getControlPoints(mesh);

				unsigned int triCount = mesh->GetPolygonCount();
				unsigned int vertexCount = 0;

				for (unsigned int i = 0; i < triCount; i++)
				{
					unsigned int polyVertexCount = mesh->GetPolygonSize(i);

					for (unsigned int j = 0; j < polyVertexCount; j++)
					{
						int controlPointIndex = mesh->GetPolygonVertex(i, j);

						XMFLOAT3& position = mPositions[controlPointIndex];
					//	XMFLOAT3 normal = fbx_getNormal(mesh, controlPointIndex, vertexCount);
					//	XMFLOAT3 binormal = fbx_getBinormals(mesh, controlPointIndex, vertexCount);
					//	XMFLOAT3 tangent = fbx_getTangents(mesh, controlPointIndex, vertexCount);
					//	XMFLOAT2 uv = fbx_getTexcoords(mesh, controlPointIndex, mesh->GetTextureUVIndex(i, j));
						
						vertexCount++;
					}
				}
			}
		}

		const int childCount = node->GetChildCount();

		for (unsigned int i = 0; i < childCount; i++)
		{
			fbx_loadNode(node->GetChild(i));
		}

	}
	void MeshLoader::fbx_getControlPoints(FbxMesh* mesh)
	{
		unsigned int count = mesh->GetControlPointsCount();

		for (unsigned int i = 0; i < count; i++)
		{
			XMFLOAT3 position;

			position.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
			position.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
			position.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);

			mPositions.emplace_back(position);
		}
	}

	void MeshLoader::fbx_toVertex(const XMFLOAT3& position, const XMFLOAT3& normal, const XMFLOAT3& binormal, const XMFLOAT3& tangent, const XMFLOAT2& texcoord)
	{

	}

	XMFLOAT3 MeshLoader::fbx_getNormal(const FbxMesh* mesh, int controlPoint, int vertex)
	{
		XMFLOAT3 result = XMFLOAT3(0, 0, 0);

		if(mesh->GetElementNormalCount() < 1)
		{
			ExceptionLog(E_FAIL, "no normal detected for this mesh.");
			return result;
		}

		const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);

		switch (vertexNormal->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (vertexNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPoint).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPoint).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPoint).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(controlPoint);

				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			}	
			break;
			}
		}
		break;
		
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (vertexNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertex).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertex).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertex).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexNormal->GetIndexArray().GetAt(vertex);
				result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			}
			break;
			}
		}
		break;

		}

		mNormals.emplace_back(result);

		return result;

	}

	XMFLOAT3 MeshLoader::fbx_getBinormals(const FbxMesh* mesh, int controlPoint, int vertex)
	{
		XMFLOAT3 result = XMFLOAT3(0, 0, 0);

		if (mesh->GetElementBinormalCount() < 1)
		{
			ExceptionLog(E_FAIL, "no normal detected for this mesh.");
			return result;
		}

		const FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);

		switch (vertexBinormal->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (vertexBinormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPoint).mData[0]);
				result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPoint).mData[1]);
				result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPoint).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexBinormal->GetIndexArray().GetAt(controlPoint);

				result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
			}
			break;
			}
		}
		break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (vertexBinormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertex).mData[0]);
				result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertex).mData[1]);
				result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertex).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexBinormal->GetIndexArray().GetAt(vertex);
				result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
			}
			break;
			}
		}
		break;

		}

		mBinormals.emplace_back(result);

		return result;

	}

	XMFLOAT3 MeshLoader::fbx_getTangents(const FbxMesh* mesh, int controlPoint, int vertex)
	{
		XMFLOAT3 result = XMFLOAT3(0, 0, 0);

		if (mesh->GetElementTangentCount() < 1)
		{
			ExceptionLog(E_FAIL, "no normal detected for this mesh.");
			return result;
		}

		const FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);

		switch (vertexTangent->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (vertexTangent->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPoint).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPoint).mData[1]);
				result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPoint).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexTangent->GetIndexArray().GetAt(controlPoint);

				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			}
			break;
			}
		}
		break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (vertexTangent->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertex).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertex).mData[1]);
				result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertex).mData[2]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexTangent->GetIndexArray().GetAt(vertex);
				result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
				result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			}
			break;
			}
		}
		break;

		}

		mTangents.emplace_back(result);

		return result;

	}

	XMFLOAT2 MeshLoader::fbx_getTexcoords(const FbxMesh* mesh, int controlPoint, int texcoordIndex)
	{
		XMFLOAT2 result = XMFLOAT2(0, 0);

		if (mesh->GetElementUVCount() < 1)
		{
			ExceptionLog(E_FAIL, "no texcoord detected for this mesh.");
			return result;
		}

		const FbxGeometryElementUV* vertexUV = mesh->GetElementUV(0);

		switch (vertexUV->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (vertexUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(controlPoint).mData[0]);
				result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(controlPoint).mData[1]);
			}

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexUV->GetIndexArray().GetAt(controlPoint);

				result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
			}
			break;
			}
		}
		break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (vertexUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(texcoordIndex).mData[0]);
				result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(texcoordIndex).mData[1]);
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
			{
				int index = vertexUV->GetIndexArray().GetAt(texcoordIndex);
				result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
				result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
			}
			break;
			}
		}
		break;

		}

		return result;
	}

	Mesh* MeshLoader::ConvertMesh() const
	{
		Mesh* mesh = new Mesh();

		D3D11_BUFFER_DESC vertexDesc{};
		D3D11_BUFFER_DESC indexDesc{};
		D3D11_SUBRESOURCE_DATA vertexSub{}, indexSub{};

		if (mVertexType == VERTEX_STATIC)
		{
			vertexDesc.ByteWidth = sizeof(Vertex_Static) * mVertices.size();
			vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexDesc.Usage = D3D11_USAGE_DEFAULT;

			vertexSub.pSysMem = mVertices.data();
		}

		indexDesc.ByteWidth = sizeof(unsigned int) * mIndices.size();
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.Usage = D3D11_USAGE_DEFAULT;

		indexSub.pSysMem = mIndices.data();

		HRESULT result = mDevice->CreateBuffer(&vertexDesc, &vertexSub, mesh->mVertex.GetAddressOf());
		ExceptionWarning(result, "Creating vertex buffer.");

		result = mDevice->CreateBuffer(&indexDesc, &indexSub, mesh->mIndex.GetAddressOf());
		ExceptionWarning(result, "Creating index buffer.");

		return mesh;

	}
}


