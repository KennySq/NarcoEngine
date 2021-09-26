#include"inc/Trasnform.h"

namespace NARCO
{
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
		XMMATRIX rot = XMMatrixRotationRollPitchYaw(x, y, z);
		XMMATRIX origin = XMLoadFloat4x4(&mMatrix);

		origin *= rot;

		XMStoreFloat4x4(&mMatrix, origin);
	}
	void Transform::Rotate(XMVECTOR offset)
	{
		XMMATRIX rot = XMMatrixRotationRollPitchYawFromVector(offset);
		XMMATRIX origin = XMLoadFloat4x4(&mMatrix);

		origin *= rot;

		XMStoreFloat4x4(&mMatrix, origin);

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

	}
	void Transform::render(float delta)
	{
	}
	void Transform::release()
	{
	}
}