#pragma once
#include"Common.h"

#include"Component.h"
namespace NARCO
{
	class Transform : public Component
	{
	public:
		Transform() : Component(typeid(this).name()) {}
		~Transform() {}

		XMMATRIX TRS(XMVECTOR translation, XMVECTOR rotation, XMVECTOR scale);

		void Translate(float x, float y, float z);
		void Translate(XMVECTOR offset);
		
		void Rotate(float x, float y, float z);
		void Rotate(XMVECTOR offset);

		void SetScale(float x, float y, float z);

		XMMATRIX GetMatrix() const { return XMLoadFloat4x4(&mMatrix); }

	private:
		XMFLOAT4X4 mMatrix;

		XMFLOAT4 mPosition;
		XMFLOAT4 mRotation;
		XMFLOAT4 mScale;

		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;

	};
}