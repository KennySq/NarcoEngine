#pragma once

#include"Prefab.h"

namespace NARCO
{
	class Dragon : public Prefab
	{
	public:
		Dragon();
		~Dragon();

		virtual void awake() override;
		virtual void start() override;
		virtual void update(float delta) override;
		virtual void render(float delta) override;
		virtual void release() override;
	
	private:


	};
}