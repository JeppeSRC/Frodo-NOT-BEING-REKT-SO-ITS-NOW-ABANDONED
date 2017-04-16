#pragma once

#include <frodo.h>

class PBRTest : public FD::FDApplication {
protected:
	void OnInit() override;
	void OnTick() override;
	void OnUpdate(float delta) override;
	void OnRender() override;
	void OnExit() override;

private:
	FD::Scene* scene;
	FD::UserCamera* camera;
	uint32 fps;
	FD::MaterialInstance* skyboxMaterial;

public:
	PBRTest() : FDApplication("PBR", 1000, 600) {}
};
