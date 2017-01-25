#pragma once

#include <frodo.h>

class PBRTest : public FDApplication {
protected:
	void OnInit() override;
	void OnTick() override;
	void OnUpdate(float delta) override;
	void OnRender() override;
	void OnExit() override;

private:
	Scene* scene;
	UserCamera* camera;
	uint32 fps;
	MaterialInstance* skyboxMaterial;

public:
	PBRTest() : FDApplication("PBR", 1000, 600) {}
};
