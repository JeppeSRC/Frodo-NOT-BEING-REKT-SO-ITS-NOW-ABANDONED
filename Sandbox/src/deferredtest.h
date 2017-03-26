#pragma once

#include <frodo.h>

class DeferredTest : public FD::FDApplication {
protected:

	void OnInit() override;
	void OnTick() override;
	void OnUpdate(float delta) override;
	void OnRender() override;
	void OnExit() override;

private:
	unsigned int fps;

	FD::String fpsString;

	FD::UserCamera* camera;

	FD::PointLight* spinningLight0;
	FD::PointLight* spinningLight1;
	FD::PointLight* spinningLight2;
	FD::PointLight* spinningLight3;
	FD::SpotLight* spotLight0;

	FD::Entity* bigSphere;

	FD::FontRenderer* fontRenderer;
	FD::Font* font;

	FD::MenuRenderer* menuRenderer;
	FD::UIHandler* handler;

public:

	DeferredTest() : FDApplication("Kebab ä gött", 1280, 720) { }



};
