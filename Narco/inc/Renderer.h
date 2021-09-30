#pragma once
#include"Component.h"
#include"GameObject.h"
#include"Scene.h"

#include"Mesh.h"
#include"Material.h"

#include"Camera.h"

namespace NARCO
{
	class Renderer : public Component
	{
	public:
		void SetMaterial(Material* material);
		void SetMesh(Mesh* mesh);

		Renderer();
		virtual ~Renderer();
	private:
		void stage_Vertex(const Material* material, const Shader* shader);
		void stage_Geometry(const Material* material, const Shader* shader);
		void stage_Domain(const Material* material, const Shader* shader);
		void stage_Hull(const Material* material, const Shader* shader);
		void stage_Pixel(const Material* material, const Shader* shader);

		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;

		ID3D11DeviceContext* mContext;
		Mesh* mMesh;


		Material* mMaterial;

		std::vector<ID3D11Buffer*> mVertexConstantBuffers;
		std::vector<ID3D11ShaderResourceView*> mVertexTextures;

		std::vector<ID3D11Buffer*> mGeometryConstantBuffers;
		std::vector<ID3D11ShaderResourceView*> mGeometryTextures;

		std::vector<ID3D11Buffer*> mDomainConstantBuffers;
		std::vector<ID3D11ShaderResourceView*> mDomainTextures;

		std::vector<ID3D11Buffer*> mHullConstantBuffers;
		std::vector<ID3D11ShaderResourceView*> mHullTextures;

		std::vector<ID3D11Buffer*> mPixelConstantBuffers;
		std::vector<ID3D11ShaderResourceView*> mPixelTextures;

		Camera* mRenderCamera;
		Transform* mTransform;
		// Mesh Data
		// Material Data;
	}; 
}