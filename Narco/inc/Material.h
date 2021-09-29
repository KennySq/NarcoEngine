#pragma once
#include"Common.h"
#include"Shader.h"

namespace NARCO
{
	enum ePropertyType
	{
		PROPERTY_CONSTANT,
		PROPERTY_BUFFER,
		PROPERTY_TEXTURE2D,
		// ...
	};

	enum ePropertyDimension
	{
		PROPERTY_SCALAR,
		PROPERTY_VECTOR2,
		PROPERTY_VECTOR3,
		PROPERTY_VECTOR4,
	};

	typedef struct MaterialProperty
	{

	public:
		MaterialProperty(const char* name, const ComPtr<ID3D11ShaderResourceView>& srv, ePropertyType type, ePropertyDimension dimension)
			: Name(name), Register(srv), PropertyType(type), Dimension(dimension) {}

		const ComPtr<ID3D11ShaderResourceView> Register;
		const char* const Name;
		
		ePropertyType PropertyType;
		ePropertyDimension Dimension;
	} MP;

	typedef struct MaterialConstantProperty
	{
		MaterialConstantProperty(const std::vector<const char*>& names, const ComPtr<ID3D11Buffer>& buffer, unsigned int size, unsigned int count)
			: Constant(buffer), Names(names), Size(size), Count(count) {}

		const ComPtr<ID3D11Buffer> Constant;
		
		std::vector<const char*> Names;

		unsigned int Count;
		unsigned int Size;
	} MCP;

	class Material
	{
	public:
		Material(Shader* shader, ID3D11Device* device, ID3D11DeviceContext* context);
		~Material();

		const Shader* GetShader() const { return mShader; }
		const std::string& GetPath() const { return mPath; }
		
		ID3D11RasterizerState* GetRasterizerState() const { return mRasterState.Get(); }
		ID3D11DepthStencilState* GetDepthStencilState() const { return mDepthState.Get(); }
		
		const auto& GetInputConstantRegisters() const { return mInputConstantProperties; }
		const auto& GetInputTextureRegisters() const { return mInputProperties; }

		void UpdateBuffer(float* v, unsigned int count, long long hash)
		{
			ID3D11Buffer* buffer;
			ID3D11ShaderResourceView* srv = mInputProperties[hash]->Register.Get();

			srv->GetResource(reinterpret_cast<ID3D11Resource**>(&buffer));

			mContext->UpdateSubresource(buffer, 0, nullptr, v, 0, 0);

			return;
		}

	private:

		HRESULT reflectVertex(ID3D11ShaderReflection* reflection);
		HRESULT reflectPixel(ID3D11ShaderReflection* reflection);

		ComPtr<ID3D11RasterizerState> mRasterState;
		ComPtr<ID3D11DepthStencilState> mDepthState;

		std::map<long long, MaterialProperty*> mInputProperties;
		std::map<long long, MaterialConstantProperty*> mInputConstantProperties;

		std::vector<ComPtr<ID3D11Buffer>> mConstantBuffers;
		std::vector<ComPtr<ID3D11Buffer>> mBuffers;
		std::vector<ComPtr<ID3D11Texture2D>> mTextures;
		
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mContext;
		std::string mPath;
		Shader* mShader;
	};
}