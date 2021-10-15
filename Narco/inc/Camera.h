#pragma once
#include"Component.h"
#include"Transform.h"
#include"GameObject.h"
#include<D3DDisplay.h>


namespace NARCO
{
	class Camera : public Component
	{
	public:
		Camera();
		~Camera();

		void SetDisplay(D3DDisplay* display);

		ID3D11Buffer* GetProjectionBuffer() const { return mBuffer.Get(); }
		ID3D11Buffer* GetViewBuffer() const { return mTransform->GetBuffer(); }

		XMMATRIX GetView() const { return mTransform->GetMatrix(); }
		XMMATRIX GetProjection() const { return XMLoadFloat4x4(&mProjection); }

		Material* RenderMaterial;
	private:
		float mFieldOfView;
		float mAspectRatio;
		float mNear;
		float mFar;

		XMFLOAT4X4 mProjection;
		ComPtr<ID3D11Buffer> mBuffer;
		
		D3DDisplay* mDisplay;


		Transform* mTransform;


		virtual void awake() override;
		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;
	};
}