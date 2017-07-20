#pragma once

#include <frodo.h>

class RTest : public FD::Application {
private:
	void OnCreateWindow() override;
	void OnInit() override;
	void OnRender() override;
	void OnUpdate(float delta) override;
	void OnTick() override;
private:
	FD::SimpleRenderer* renderer;
	FD::UserCamera* camera;
	FD::Entity3D* cube;
	FD::Entity3D* floor;

	uint32 fps;
public:
	RTest() : Application() {}
};