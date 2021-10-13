#pragma once
#include"Prefab.h"

using namespace NARCO;

class Skater : public Prefab
{
public:
	Skater();
	~Skater();

	virtual void awake() override;
	virtual void start() override;
	virtual void update(float delta) override;
	virtual void render(float delta) override;
	virtual void release() override;
};