#pragma once
#include"Common.h"
#include"Component.h"
namespace NARCO
{
	class Transform : public Component
	{
	public:
		Transform();
		~Transform() {}

		XMMATRIX TRS(XMVECTOR translation, XMVECTOR rotation, XMVECTOR scale);

		void Translate(float x, float y, float z);
		void Translate(XMVECTOR offset);
		
		void Rotate(float x, float y, float z);
		void Rotate(XMVECTOR offset);

		void SetScale(float x, float y, float z);

		XMMATRIX GetMatrix() const { return XMLoadFloat4x4(&mMatrix); }
		void SetMatrix(XMMATRIX matrix) { XMStoreFloat4x4(&mMatrix, XMMatrixTranspose(matrix)); }
		ID3D11Buffer* GetBuffer() const { return mBuffer.Get(); }

		XMVECTOR GetPosition() const { return XMLoadFloat4(&mPosition); }
		XMVECTOR GetRotation() const { return XMLoadFloat4(&mRotation); }
		XMVECTOR GetScale() const { return XMLoadFloat4(&mScale); }
		
		
	private:
		ComPtr<ID3D11Buffer> mBuffer;

		XMFLOAT4X4 mMatrix;

		XMFLOAT4 mPosition;
		XMFLOAT4 mRotation;
		XMFLOAT4 mScale;

		virtual void awake() override;
		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;
	};
}