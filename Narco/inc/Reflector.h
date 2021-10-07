#pragma once
#include<Common.h>
#include<d3d11shader.h>
#include<d3dcompiler.h>
#include<D3DTexture2D.h>


namespace NARCO
{

	enum ePropertyType
	{
		PROPERTY_UNKNOWN,
		PROPERTY_CONSTANT,
		PROPERTY_BUFFER,
		PROPERTY_TEXTURE1D,
		PROPERTY_TEXTURE2D,
		PROPERTY_TEXTURE3D,
		PROPERTY_BYTEADDRESS,
		PROPERTY_STRUCTURED,
		// ...
	};

	enum ePropertyDimension
	{
		DIMENSION_UNKNOWN,
		DIMENSION_FLOAT,
		DIMENSION_FLOAT2,
		DIMENSION_FLOAT3,
		DIMENSION_FLOAT4,
		DIMENSION_FLOAT4X4,
		DIMENSION_UINT,
		DIMENSION_UINT2,
		DIMENSION_UINT3,
		DIMENSION_UINT4,
	};

	typedef struct MaterialProperty
	{

		MaterialProperty(const char* name, const ComPtr<ID3D11ShaderResourceView>& srv, ePropertyType type, ePropertyDimension dimension)
			: Name(name), Register(srv), PropertyType(type), Dimension(dimension) {}

		void SetSRV(ID3D11ShaderResourceView* srv) { Register = srv; }

		ComPtr<ID3D11ShaderResourceView> Register;
		const char* const Name;

		ePropertyType PropertyType;
		ePropertyDimension Dimension;

		bool bUpdated;
	} MP;

	typedef struct MaterialUnorderProperty
	{

		MaterialUnorderProperty(const char* name, const ComPtr<ID3D11UnorderedAccessView>& uav, ePropertyType type, ePropertyDimension dimension)
			: Name(name), Register(uav), PropertyType(type), Dimension(dimension) {}

		void SetUAV(ID3D11UnorderedAccessView* uav) { Register = uav; }

		ComPtr<ID3D11UnorderedAccessView> Register;

		const char* const Name;

		ePropertyType PropertyType;
		ePropertyDimension Dimension;

		bool bUpdated;

	} MUP;

	typedef struct MaterialConstantProperty
	{
		struct Variable
		{
			uint Size;
			ePropertyDimension Dimension;
		};
		MaterialConstantProperty(const char* bufferName, const std::map<const char*, Variable>& names, const ComPtr<ID3D11Buffer>& buffer, unsigned int size)
			: Name(bufferName), Buffer(buffer), VariableNames(names), Size(size) {}

		const ComPtr<ID3D11Buffer> Buffer;

		const char* Name;
		std::map<const char*, Variable> VariableNames; // Key : variable name, value : variable size and type
		// std::map<const char*, DataType> Variables

		unsigned int Size;

		bool bUpdated;
	} MCP;

	typedef struct MaterialOutputProperty
	{
		MaterialOutputProperty(const char* name, const ComPtr<ID3D11RenderTargetView>& rtv, ePropertyDimension dimension)
			: Name(name), Register(rtv), Dimension(dimension)
		{}

		const char* Name;
		const ComPtr<ID3D11RenderTargetView> Register;
		ePropertyDimension Dimension;

		bool bUpdated;

	} MOP;


	class Reflector
	{
	public:
		Reflector(ID3D11Device* device, ID3DBlob* blob);
		~Reflector();

		ComPtr<ID3D11InputLayout> ReflectInputLayout(ID3D11Device* device, ID3DBlob* blob);
		MaterialConstantProperty* ReflectConstantBuffer(ID3D11Device* device, uint i);
		MaterialProperty* ReflectTexture(ID3D11Device* device, uint i);
		MaterialUnorderProperty* ReflectUnorderedAccess(ID3D11Device* device, uint i);
		MaterialOutputProperty* ReflectRenderTarget(ID3D11Device* device, uint i);

		uint GetConstBufferCount() const { return mConstantBufferCount; }
		uint GetBoundResourceCount() const { return mBoundResourceCount; }
		uint GetUnorderedCount() const { return mUnorderedCount; }
		uint GetSamplerCount() const { return mSamplerCount; }

		const auto& GetSRV() const { return mRawTextures; }
		const auto& GetUAV() const { return mRawUnorderAccesses; }
		const auto& GetBuffers() const { return mRawConstBuffers; }

		const auto& GetBufferMap() const { return mConstBuffers; }
		const auto& GetSRVMap() const { return mTextures; }
		const auto& GetUAVMap() const { return mUnorderedAcceses; }

		void AddSRV(MP* mp); //{ mRawTextures.emplace_back(mp->Register.Get()); }
		void AddUAV(MUP* mup); //{ mRawUnorderAccesses.emplace_back(mup->Register.Get()); }
		void AddBuffer(MCP* mcp); // { mRawConstBuffers.emplace_back(mcp->Buffer.Get()); }
	private:
		

		void determineVariableChannel(ID3D11ShaderReflectionType* type, ePropertyDimension& dimension);
		void determineTextureType(D3D_RESOURCE_RETURN_TYPE type, ePropertyDimension& dim);
		ComPtr<ID3D11ShaderReflection> mReflection;

		uint mConstantBufferCount;
		uint mBoundResourceCount;
		uint mUnorderedCount;
		uint mSamplerCount;

		std::map<long long, MCP*> mConstBuffers;
		std::map<long long, MP*> mTextures;
		std::map<long long, MUP*> mUnorderedAcceses;

		std::vector<ID3D11ShaderResourceView*> mRawTextures;
		std::vector<ID3D11Buffer*> mRawConstBuffers;
		std::vector<ID3D11UnorderedAccessView*> mRawUnorderAccesses;
		//std::vector<ID3D11SamplerState*> mSamplerStates;
		
		//std::vector<ComPtr<ID3D11ShaderResourceView>> mShaderResources;
		//std::vector<ComPtr<ID3D11RenderTargetView>> mRenderTargets;
		//std::vector<ComPtr<ID3D11UnorderedAccessView>> mUnorderedAccess;
		
	};
}