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

		XMStoreFloat4x4(&mMatrix, XMMatrixTranspose(origin));
	}
	void Transform::Rotate(float x, float y, float z)
	{
		float xR = XMConvertToRadians(x);
		float yR = XMConvertToRadians(y);
		float zR = XMConvertToRadians(z);

		XMMATRIX rot = XMMatrixRotationRollPitchYaw(xR, yR, zR);
		XMMATRIX origin = XMLoadFloat4x4(&mMatrix);

		origin *= rot;

		XMStoreFloat4x4(&mMatrix, XMMatrixTranspose(origin));
	}
	void Transform::Rotate(XMVECTOR offset)
	{
		float xR = XMConvertToRadians(offset.m128_f32[0]);
		float yR = XMConvertToRadians(offset.m128_f32[1]);
		float zR = XMConvertToRadians(offset.m128_f32[2]);

		XMMATRIX rot = XMMatrixRotationRollPitchYaw(xR, yR, zR);
		XMMATRIX origin = XMLoadFloat4x4(&mMatrix);

		origin *= rot;

		XMStoreFloat4x4(&mMatrix, XMMatrixTranspose(origin));

	}
	void Transform::SetScale(float x, float y, float z)
	{
		mMatrix._11 = x;
		mMatrix._22 = y;
		mMatrix._33 = z;
	}
	void Transform::start()
	{
	}
	void Transform::update(float delta)
	{
		XMVECTOR translation, rotQuat, scale;
		XMMATRIX mat = XMLoadFloat4x4(&mMatrix);
		XMMatrixDecompose(&scale, &rotQuat, &translation, mat);

		XMStoreFloat4(&mPosition, translation);
		XMStoreFloat4(&mRotation, rotQuat);
		XMStoreFloat4(&mScale, scale);

		mContext->UpdateSubresource(mBuffer.Get(), 0, nullptr, &mMatrix, 0, 0);

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