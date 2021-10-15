#pragma once
#include"Material.h"
#include"Mesh.h"
#include"Component.h"
namespace NARCO
{
	class Renderer : public Component
	{
	public:
		Renderer();
		~Renderer();

		void SetMesh(Mesh* mesh) { mMesh = mesh; }
		void AddMaterial(Material* material) { mMaterials.emplace_back(material); }
		void RemoveMaterial(uint index) { mMaterials[index] = nullptr; }

		void UpdateSharedResources(Material* mat);

	private:
		virtual void awake() override;
		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;

		Mesh* mMesh;
		std::vector<Material*> mMaterials;
	};
}
