#pragma once
#include"Component.h"

#include"Mesh.h"
#include"Material.h"

namespace NARCO
{
	class Renderer : public Component
	{
	public:
		Renderer(ID3D11DeviceContext* context);
		virtual ~Renderer();
	private:
		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;

		ID3D11DeviceContext* mContext;
		Mesh* mMesh;
		std::vector<Material*> mMaterials;
		// Mesh Data
		// Material Data;
	}; 
}