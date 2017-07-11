#pragma once

#include <frodo.h>

class PBRTest : public FD::Application {
protected:
	void OnCreateWindow() override;
	void OnInit() override;
	void OnTick() override;
	void OnUpdate(float delta) override;
	void OnRender() override;
	void OnExit() override;

private:
	FD::PointLight* light;
	FD::Scene* scene;
	FD::UserCamera* camera;
	uint32 fps;
	FD::MaterialInstance* skyboxMaterial;
	FD::Audio* audio;
	FD::AudioMixer* mainMixer;

public:
	PBRTest() : Application() {}
};
