#pragma once
#include"Common.h"
#include"GameObject.h"
#include"Prefab.h"

namespace NARCO
{
	class BackPlane : public Prefab
	{
	public:
		BackPlane();
		~BackPlane();

		virtual void awake() override;
		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;
	};
}

