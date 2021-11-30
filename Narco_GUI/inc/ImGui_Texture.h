#pragma once

#include<Common.h>


class ImGui_Texture
{
    PUBLIC_API NARCO_API ImGui_Texture(ID3D11Device* device, const char* path);

    PUBLIC_API NARCO_API ID3D11ShaderResourceView* GetSRV() const { return mShaderResourceView.Get(); }
    PUBLIC_API NARCO_API int GetWidth() const { return mWidth; }
    PUBLIC_API NARCO_API int GetHeight() const { return mHeight; }
    PUBLIC_API NARCO_API const std::string& GetFileName() const { return mFileName; }

    PUBLIC_API NARCO_API ID3D11ShaderResourceView* Migrate()
    {
        ID3D11ShaderResourceView* srv = mShaderResourceView.Detach();
        mShaderResourceView = nullptr;

        return srv;
    }

    PRIVATE_PROPERTY ComPtr<ID3D11ShaderResourceView> mShaderResourceView;

    PRIVATE_PROPERTY std::string mFileName;

    PRIVATE_PROPERTY int mWidth;
    PRIVATE_PROPERTY int mHeight;
};