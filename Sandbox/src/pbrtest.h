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
	PBRStaticRenderer* renderer;
	UserCamera* camera;
	

public:
	PBRTest() : FDApplication("PBR", 1000, 600) {}
};
