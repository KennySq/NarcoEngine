#include"inc/Transform.h"

namespace NARCO
{
	Transform::Transform()
	{
		XMMATRIX origin = XMMatrixIdentity();

		XMStoreFloat4x4(&mMatrix, origin);

		D3D11_BUFFER_DESC bufferDesc{};
		D3D11_SUBRESOURCE_DATA subData{};

		bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		subData.pSysMem = &mMatrix;

		HRESULT result = mDevice->CreateBuffer(&bufferDesc, &subData, mBuffer.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log("failed to create transform buffer.\n size : " +
				std::to_string(bufferDesc.ByteWidth));
			return;
		}

	}

	// TRS will return transform matrix based on current rotation
	XMMATRIX Transform::TRS(XMVECTOR translation, XMVECTOR rotation, XMVECTOR scale)
	{
		XMVECTOR rotateOrigin = XMLoadFloat4(&mRotation);
		XMMATRIX trs = XMMatrixAffineTransformation(scale, rotateOrigin, rotation, translation);
	
		return trs;
	}
	void Transform::Translate(float x, float y, float z)
	{
		XMMATRIX translation = XMMatrixTranslation(x, y, z);

		XMMATRIX origin = XMLoadFloat4x4(&mMatrix);

		origin *= translation;

		XMStoreFloat4x4(&mMatrix, origin);
	}
	void Transform::Translate(XMVECTOR offset)
	{

		XMMATRIX translation = XMMatrixTranslationFromVector(offset);

		XMMATRIX origin = XMLoadFloat4x4(&mMatrix);
		

		origin *= translation;

		XMStoreFloat4x4(&mMatrix, origin);
	}
	void Transform::Rotate(float x, float y, float z)
	{
		float xR = XMConvertToRadians(x);
		float yR = XMConvertToRadians(y);
		float zR = XMConvertToRadians(z);

		
		XMVECTOR rotQuat = XMQuaternionRotationRollPitchYaw(xR, yR, zR);
		XMMATRIX rot = XMMatrixRotationQuaternion(rotQuat);

		XMMATRIX translation = XMMatrixTranslation(mMatrix._41, mMatrix._42, mMatrix._43);

		XMMATRIX origin = rot;

		origin *= translation;

		XMStoreFloat4x4(&mMatrix, origin);
	}
	void Transform::Rotate(XMVECTOR offset)
	{
		float xR = XMConvertToRadians(offset.m128_f32[0]);
		float yR = XMConvertToRadians(offset.m128_f32[1]);
		float zR = XMConvertToRadians(offset.m128_f32[2]);

		XMMATRIX rot;
		XMMATRIX rotX = XMMatrixRotationX(xR);
		XMMATRIX rotY = XMMatrixRotationY(yR);
		XMMATRIX rotZ = XMMatrixRotationZ(zR);

		rot = XMMatrixMultiply(rotX, rotY);
		rot = XMMatrixMultiply(rot, rotZ);

		XMMATRIX origin = XMLoadFloat4x4(&mMatrix);

		XMMatrixMultiply(origin, rot);

		XMStoreFloat4x4(&mMatrix, origin);

	}
	void Transform::SetScale(float x, float y, float z)
	{
		mMatrix._11 = x;
		mMatrix._22 = y;
		mMatrix._33 = z;
	}
	void Transform::SetPosition(float x, float y, float z)
	{
		mMatrix._41 = x;
		mMatrix._42 = y;
		mMatrix._43 = z;
	}
	void Transform::SetRotation(float x, float y, float z)
	{
		float xR = XMConvertToRadians(x);
		float yR = XMConvertToRadians(y);
		float zR = XMConvertToRadians(z);

		XMMATRIX rotQuat = XMMatrixRotationRollPitchYaw(xR, yR, zR);
		XMVECTOR outScale, outPosition, outRotation;
		XMMATRIX origin = XMLoadFloat4x4(&mMatrix);

		if (XMMatrixDecompose(&outScale, &outRotation, &outPosition, origin))
		{
			Debug::Log("failed to decompose transform matrix");
		}

		XMMATRIX rot = XMMatrixRotationQuaternion(-outRotation);

		XMMatrixMultiply(origin, rot);

		XMStoreFloat4x4(&mMatrix, origin);
	}
	void Transform::start()
	{
	}
	void Transform::update(float delta)
	{
		if (RenderMaterial != nullptr)
		{
			static const char* cbufferName = "Constants";
			static const char* variableName = "gWorld";
			XMMATRIX loadMat = XMMatrixTranspose(XMLoadFloat4x4(&mMatrix));

			RenderMaterial->MapConstantBuffer(cbufferName, variableName, RESOURCE_CBUFFER, &loadMat, sizeof(loadMat));	
		}

		XMVECTOR translation, rotQuat, scale;
		XMMATRIX mat = XMLoadFloat4x4(&mMatrix);
		XMMatrixDecompose(&scale, &rotQuat, &translation, mat);

		XMStoreFloat4(&mPosition, translation);
		XMStoreFloat4(&mRotation, rotQuat);
		XMStoreFloat4(&mScale, scale);
		
	//	mContext->UpdateSubresource(mBuffer.Get(), 0, nullptr, &loadMat, 0, 0);
	}
	void Transform::render(float delta)
	{
	}
	void Transform::release()
	{
	}
	void Transform::awake()
	{
	}

}