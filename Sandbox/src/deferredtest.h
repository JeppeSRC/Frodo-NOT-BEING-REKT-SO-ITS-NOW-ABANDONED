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
	Entity* e;
	unsigned int fps;

	UserCamera* camera;

public:

	DeferredRenderer* renderer;

	DeferredTest() : FDApplication("Kebab ä gött", 1000, 600) { }



};
