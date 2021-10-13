#pragma once
#include"Component.h"
#include"GameObject.h"
#include"Scene.h"

#include"Mesh.h"
#include"Material.h"

#include"GBuffer.h"

#include"Camera.h"

#include"Narco_Core.h"

#include"Light.h"

namespace NARCO
{
	class Renderer : public Component
	{
	public:
		void AddMaterial(Material* material);
		void SetMaterial(Material* material, uint index);
		void SetMesh(Mesh* mesh);

		Material* GetMaterial(uint index) const { return mMaterials[index]; }
		Mesh* GetMesh() const { return mMesh; }
		Renderer();
		virtual ~Renderer();
	private:

		void stage_UpdateReservedResources(const Material* material, const Shader* shader, uint materialID);

		void stage_InputAssembly(const Mesh* mesh, const Shader* shader);

		void stage_Vertex(const Material* material, const Shader* shader);
		void stage_Geometry(const Material* material, const Shader* shader);
		void stage_Domain(const Material* material, const Shader* shader);
		void stage_Hull(const Material* material, const Shader* shader);
		void stage_Pixel(const Material* material, const Shader* shader);

		void stage_ReflectVertex(const Material* material, const Shader* shader);
		void stage_ReflectGeometry(const Material* material, const Shader* shader);
		void stage_ReflectDomain(const Material* material, const Shader* shader);
		void stage_ReflectHull(const Material* material, const Shader* shader);
		void stage_ReflectPixel(const Material* material, const Shader* shader);

		virtual void awake() override;
		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;

		ID3D11DeviceContext* mContext;
		Mesh* mMesh;

		std::vector<Material*> mMaterials;
		Transform* mTransform;

		Camera* mRenderCamera;
		GBuffer* mGBuffer;

		std::vector<Light*> mEffectLights;
	}; 
}