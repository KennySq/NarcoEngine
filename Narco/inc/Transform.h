#pragma once
#include"Common.h"
#include"Component.h"

#include"Material.h"

namespace NARCO
{

	//class ProxyTransform : public MaterialProxy
	//{
	//public:
	//	ProxyTransform(Material* material)
	//		: mMaterial(material) {	}


	//	virtual void Invoke() override;
	//private:

	//	Material* mMaterial;
	//	const char* mName;
	//	const char* mVariable;
	//	eResourceType mType;
	//	void* mData;
	//	uint mSize;

	//};

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
		void SetPosition(float x, float y, float z);

		void SetRotation(float x, float y, float z);

		XMMATRIX GetMatrix() const { return XMLoadFloat4x4(&mMatrix); }
		void SetMatrix(XMMATRIX matrix) { XMStoreFloat4x4(&mMatrix, matrix); }
		ID3D11Buffer* GetBuffer() const { return mBuffer.Get(); }

		XMVECTOR GetPosition() const { return XMLoadFloat4(&mPosition); }
		XMVECTOR GetRotation() const { return XMLoadFloat4(&mRotation); }
		XMVECTOR GetScale() const { return XMLoadFloat4(&mScale); }
		
		Material* RenderMaterial;
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