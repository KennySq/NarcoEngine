#include"inc/Camera.h"

namespace NARCO
{
	Camera::Camera()
		: mFieldOfView(XMConvertToRadians(90.0f)), mFar(1000.0f), mNear(0.01f), mAspectRatio(1.333f)
	{
		auto hw = D3DHW::GetInstance();
		
		D3D11_SUBRESOURCE_DATA subData{};
		XMMATRIX projection = XMMatrixPerspectiveFovLH(mFieldOfView, mAspectRatio, mNear, mFar);
		XMStoreFloat4x4(&mProjection, XMMatrixTranspose(projection));
		
		D3D11_BUFFER_DESC bufferDesc{};

		subData.pSysMem = &mProjection;

		bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT result = mDevice->CreateBuffer(&bufferDesc, &subData, mBuffer.GetAddressOf());
		if (result != S_OK)
		{
			Debug::Log(mName + "failed to create projection matrix buffer.");
			return;
		}
	}
	Camera::~Camera()
	{
	}
	void Camera::SetDisplay(D3DDisplay* display)
	{
		mDisplay = display;

		const D3D11_VIEWPORT& viewport = mDisplay->GetMainViewport();

		float width = viewport.Width;
		float height = viewport.Height;

		mAspectRatio = width / height;
		XMMATRIX projection = XMMatrixPerspectiveFovLH(mFieldOfView, mAspectRatio, mNear, mFar);
		XMStoreFloat4x4(&mProjection, XMMatrixTranspose(projection));


	}

	void Camera::awake()
	{
		mTransform = mRoot->GetComponent<Transform>();
		
		XMVECTOR eye, up, at;

		eye = XMVectorSet(-1, 0, 0, 0);
		up = XMVectorSet(0, 1, 0, 0);
		at = XMVectorSet(0, 0, 0, 0);
		XMMATRIX view = XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up));

		mTransform->SetMatrix(view);

	}

	void Camera::start()
	{

		
		
	}
	void Camera::update(float delta)
	{
		ID3D11Buffer* view = mTransform->GetBuffer();
		ID3D11Buffer* proj = mBuffer.Get();



		mContext->UpdateSubresource(proj, 0, nullptr, &mProjection, 0, 0);
		
	}
	void Camera::render(float delta)
	{
	}
	void Camera::release()
	{
	}

}