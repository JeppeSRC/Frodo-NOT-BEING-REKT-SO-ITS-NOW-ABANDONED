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

public:

	DeferredRenderer* renderer;

	DeferredTest() : FDApplication("Deferred", 1000, 600) { }



};
