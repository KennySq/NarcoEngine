#include "inc/MeshLoader.h"

#include<fbxsdk.h>

#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

#define FBXSDK_SHARED


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
			Debug::Log(mPath + " is invalid");
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
			Debug::Log("FbxImporter initialization failed.");
			return false;
		}

		FbxScene* scene = FbxScene::Create(manager, "Scene");
		importer->Import(scene);

		//fbxsdk::FbxAxisSystem::DirectX.ConvertScene(scene);

		FbxGeometryConverter geometryConverter(manager);
		geometryConverter.Triangulate(scene, true);


		auto nodeCount = scene->GetNodeCount();
		FbxNode* rootNode = scene->GetRootNode();

		fbx_loadNode(rootNode);

		importer->Destroy();

		return true;
	}
	void MeshLoader::fbx_loadNode(FbxNode* node)
	{
		FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

		if (nodeAttribute != nullptr)
		{
			FbxNodeAttribute::EType attr = nodeAttribute->GetAttributeType();
			if (attr == FbxNodeAttribute::eMesh)
			{
				FbxMesh* mesh = node->GetMesh();
				fbx_getControlPoints(mesh);

				unsigned int triCount = mesh->GetPolygonCount();
				unsigned int vertexCount = 0;

				mTotalTriangles += triCount;

				for (unsigned int i = 0; i < triCount; i++)
				{
					unsigned int polyVertexCount = mesh->GetPolygonSize(i);

					for (unsigned int j = 0; j < polyVertexCount; j++)
					{
						int controlPointIndex = mesh->GetPolygonVertex(i, j);

						XMFLOAT3 normal{};
						XMFLOAT3 binormal{};
						XMFLOAT3& position = mPositions[controlPointIndex];
						XMFLOAT3 tangent{};
						XMFLOAT2 uv{};
						uint materialID = 0;

						if (mesh->GetElementNormalCount() >= 1)
						{
							normal = fbx_getNormal(mesh, controlPointIndex, vertexCount);
						}

						if (mesh->GetElementBinormalCount() >= 1)
						{
							binormal = fbx_getBinormals(mesh, controlPointIndex, vertexCount);
						}

						if (mesh->GetElementTangentCount() >= 1)
						{
							tangent = fbx_getTangents(mesh, controlPointIndex, vertexCount);
						}

						if (mesh->GetElementUVCount() >= 1)
						{
							uv = fbx_getTexcoords(mesh, controlPointIndex, i * polyVertexCount + j);
						}

						if (mesh->GetElementMaterialCount() >= 1)
						{
							materialID = fbx_getMaterialID(mesh, i);
						}

						Vertex_Static vertex;


						vertex.mPosition = position;
						vertex.mNormal = normal;
						vertex.mBinormal = binormal;
						vertex.mTangent = tangent;
						vertex.mTexcoord = uv;
						vertex.mMaterialID = materialID;

						fbx_insertVertex(position, normal, binormal, tangent, uv, materialID, vertexCount);
						vertexCount++;


					}
				}
				mPositions.clear();
				mMeshCount++;
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

		if (mesh->GetElementNormalCount() < 1)
		{
			Debug::Log("no normal detected for this mesh. ");
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
			Debug::Log("no binormal detected for this mesh.");
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
			Debug::Log("no tangent detected for this mesh.");
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
			Debug::Log("no texcoord detected for this mesh.");
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
			break;

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

				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertexUV->GetIndexArray().GetAt(texcoordIndex);
					result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
					result.y = 1.0f - static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
				}
				break;
			}
		}
		break;

		}

		return result;
	}

	uint MeshLoader::fbx_getMaterialID(const FbxMesh* mesh, int polygon)
	{
		uint result = 0;

		for (uint i = 0; i < mesh->GetLayerCount(); i++)
		{
			const fbxsdk::FbxLayerElementMaterial* layerMat = mesh->GetLayer(i)->GetMaterials();
			
			if (layerMat == nullptr)
			{
				continue;
			}

			result = layerMat->GetIndexArray().GetAt(polygon);
		}

		mMaterialIDs.emplace_back(result);

		return result;

	}

	void MeshLoader::fbx_insertVertex(const XMFLOAT3& position, const XMFLOAT3& normal, const XMFLOAT3& binormal, const XMFLOAT3& tangent, const XMFLOAT2& uv, int materialID, unsigned int vertexCount)
	{
		Vertex_Static vertex = { position , normal, binormal, tangent, uv, materialID };

		auto lookup = mIndexMap.find(vertex);

		if (lookup != mIndexMap.end())
		{
			mIndices.push_back(lookup->second);
		}
		else
		{
			unsigned int index = mVertices.size();
			mIndexMap[vertex] = index;
			mIndices.push_back(index);
			mVertices.push_back(vertex);
		}
	}

	Mesh* MeshLoader::ConvertMesh() const
	{
		std::string fileName = mPath.substr(mPath.find_last_of("/") + 1);

		Mesh* mesh = new Mesh(fileName);

		D3D11_BUFFER_DESC vertexDesc{};
		D3D11_BUFFER_DESC indexDesc{};
		D3D11_SUBRESOURCE_DATA vertexSub{}, indexSub{};

		if (mVertexType == VERTEX_STATIC)
		{
			vertexDesc.ByteWidth = sizeof(Vertex_Static) * mVertices.size();
			vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexDesc.Usage = D3D11_USAGE_DEFAULT;

			vertexSub.pSysMem = mVertices.data();

			mesh->mStride = sizeof(Vertex_Static);
		}

		indexDesc.ByteWidth = sizeof(unsigned int) * mIndices.size();
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.Usage = D3D11_USAGE_DEFAULT;

		indexSub.pSysMem = mIndices.data();

		mesh->mIndexCount = mIndices.size();

		HRESULT result = mDevice->CreateBuffer(&vertexDesc, &vertexSub, mesh->mVertex.GetAddressOf());

		if (result != S_OK)
		{
			Debug::Log("failed to create vertex buffer.\n size : " + vertexDesc.ByteWidth);

			delete mesh;
			return nullptr;
		}

		result = mDevice->CreateBuffer(&indexDesc, &indexSub, mesh->mIndex.GetAddressOf());

		if (result != S_OK)
		{
			Debug::Log("failed to create index buffer.\n size : " + indexDesc.ByteWidth);


			delete mesh;
			return nullptr;
		}

		return mesh;

	}
}


