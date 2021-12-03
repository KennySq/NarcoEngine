#include"inc/Material.h"

namespace NARCO
{
	LightHandler* Material::mLightHandler = nullptr;

	struct InputLayoutMaker
	{
		ComPtr<ID3D11InputLayout> operator()(ID3D11Device* device, Stage<ID3D11VertexShader>* vertexStage)
		{
			HRESULT result;
			D3D11_SHADER_DESC shaderDesc{};

			ID3D11ShaderReflection* reflection = vertexStage->GetReflection();
			ID3DBlob* byteCode = vertexStage->GetByteCode();

			std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;

			result = reflection->GetDesc(&shaderDesc);
			if (result != S_OK)
			{
				Debug::Log("failed to get shader descriptor.");
			}

			for (unsigned int i = 0; i < shaderDesc.InputParameters; i++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC parameterDesc{};

				reflection->GetInputParameterDesc(i, &parameterDesc);

				D3D11_INPUT_ELEMENT_DESC elementDesc{};

				elementDesc.SemanticName = parameterDesc.SemanticName;
				elementDesc.SemanticIndex = parameterDesc.SemanticIndex;
				elementDesc.InputSlot = 0;
				elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				elementDesc.InstanceDataStepRate = 0;

				if (parameterDesc.Mask == 1)
				{
					if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32_UINT;
					}
					else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
					}
					else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32_SINT;
					}
				}
				else if (parameterDesc.Mask <= 3)
				{
					if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
					}
					else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
					}
					else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
					}
				}
				else if (parameterDesc.Mask <= 7)
				{
					if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
					}
					else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
					}
					else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
					}
				}
				else if (parameterDesc.Mask <= 15)
				{
					if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
					}
					else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
					}
					else if (parameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					{
						elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
					}
				}

				inputElements.emplace_back(elementDesc);

			}

			ComPtr<ID3D11InputLayout> il;

			result = device->CreateInputLayout(inputElements.data(), inputElements.size(), byteCode->GetBufferPointer(), byteCode->GetBufferSize(), il.GetAddressOf());
			if (result != S_OK)
			{
				Debug::Log("failed to create input layout");
				return nullptr;
			}

			return il;
		}
	};

	Material::Material(const char* shaderPath, uint stageFlags, bool bLit)
		: mShaderPath(shaderPath), mStageFlags(stageFlags)
	{
		mFileName = mShaderPath.substr(mShaderPath.find_last_of('/') + 1);
		mLightHandler = LightHandler::GetInstance();

		if (stageFlags & STAGE_VERTEX)
		{
			mVertexStage = new Stage<ID3D11VertexShader>(shaderPath);
			mVertexStage->Reflect(&mBuffers);
			mVertexStage->Reflect(&mShaderResources);
			mInputLayout = InputLayoutMaker{}(mDevice, mVertexStage);
		}

		if (stageFlags & STAGE_GEOMETRY)
		{
			mGeometryStage = new Stage<ID3D11GeometryShader>(shaderPath);
			mGeometryStage->Reflect(&mBuffers);
			mGeometryStage->Reflect(&mShaderResources);
		}

		if (stageFlags & STAGE_DOMAIN)
		{
			mDomainStage = new Stage<ID3D11DomainShader>(shaderPath);
			mDomainStage->Reflect(&mBuffers);
			mDomainStage->Reflect(&mShaderResources);
		}

		if (stageFlags & STAGE_GEOMETRY)
		{
			mHullStage = new Stage<ID3D11HullShader>(shaderPath);
			mHullStage->Reflect(&mBuffers);
			mHullStage->Reflect(&mShaderResources);
		}

		if (stageFlags & STAGE_PIXEL)
		{
			static const std::vector<std::string> classInstances
			{
				"gPointLightInstances",
				"gDirectionalLightInstances",
			};

			mPixelStage = new Stage<ID3D11PixelShader>(shaderPath, mLightHandler->GetLinkage());
			mPixelStage->Reflect(&mBuffers);
			mPixelStage->Reflect(&mShaderResources);
			if (bLit == true)
			{
				mPixelStage->Reflect(&mClassInstances, mLightHandler->GetLinkage(), "gLightInstanceSlot", classInstances);
			}

		}

	}
	Material::~Material()
	{
	}

	Material::Material(const Material& material)
		: mStageFlags(material.mStageFlags), mShaderPath(material.mShaderPath)
	{
		if (this != nullptr)
		{
			if (mVertexStage != nullptr)
			{
				delete mVertexStage;
			}

			if (mGeometryStage != nullptr)
			{
				delete mGeometryStage;
			}

			if (mDomainStage != nullptr)
			{
				delete mDomainStage;
			}

			if (mHullStage != nullptr)
			{
				delete mHullStage;
			}

			if (mPixelStage != nullptr)
			{
				delete mPixelStage;
			}
		}


		mFileName = mShaderPath.substr(mShaderPath.find_last_of('/') + 1);

		if (mStageFlags & STAGE_VERTEX)
		{
			mVertexStage = new Stage<ID3D11VertexShader>(*material.mVertexStage);
			mVertexStage->Reflect(&mBuffers);
			mVertexStage->Reflect(&mShaderResources);
			mInputLayout = material.mInputLayout;
		}

		if (mStageFlags & STAGE_GEOMETRY)
		{
			mGeometryStage = new Stage<ID3D11GeometryShader>(*material.mGeometryStage);
			mGeometryStage->Reflect(&mBuffers);
			mGeometryStage->Reflect(&mShaderResources);
		}

		if (mStageFlags & STAGE_DOMAIN)
		{
			mDomainStage = new Stage<ID3D11DomainShader>(*material.mDomainStage);
			mDomainStage->Reflect(&mBuffers);
			mDomainStage->Reflect(&mShaderResources);
		}

		if (mStageFlags & STAGE_HULL)
		{
			mHullStage = new Stage<ID3D11HullShader>(*material.mHullStage);
			mHullStage->Reflect(&mBuffers);
			mHullStage->Reflect(&mShaderResources);
		}

		if (mStageFlags & STAGE_PIXEL)
		{
			mPixelStage = new Stage<ID3D11PixelShader>(*material.mPixelStage);
			mPixelStage->Reflect(&mBuffers);
			mPixelStage->Reflect(&mShaderResources);
		}

	}

	Material* Material::MakeInstance()
	{
		Material* instance = new Material(*this);

		instance->mFileName += " (Instance)";

		return instance;
	}

}