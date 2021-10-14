#pragma once
#include"Prefab.h"
#include"MeshLoader.h"

using namespace NARCO;

class Shiba : public Prefab
{
public:

	Shiba();
	~Shiba();

	virtual void awake() override;
	virtual void start() override;
	virtual void update(float delta) override;
	virtual void render(float delta) override;
	virtual void release() override;
};
