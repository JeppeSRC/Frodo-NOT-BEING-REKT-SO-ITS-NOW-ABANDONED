#pragma once

#include <frodo.h>

class DeferredTest : public FDApplication {
protected:

	void OnInit() override;
	void OnTick() override;
	void OnUpdate(float delta) override;
	void OnRender() override;
	void OnExit() override;

private:
	unsigned int fps;

	UserCamera* camera;

	PointLight* spinningLight0;
	PointLight* spinningLight1;
	PointLight* spinningLight2;
	PointLight* spinningLight3;
	SpotLight* spotLight0;

	Entity* bigSphere;


public:

	DeferredTest() : FDApplication("Kebab � g�tt", 1000, 600) { }



};
