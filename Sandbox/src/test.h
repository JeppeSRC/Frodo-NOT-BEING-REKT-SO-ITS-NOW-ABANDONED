#pragma once
#include <frodo.h>

class Test : public FD::FDApplication {
protected:

	void OnInit() override;
	void OnTick() override;
	void OnUpdate(float delta) override;
	void OnRender() override;
	void OnExit() override;

public:
	FD::FontRenderer* renderer;
	FD::Font* font;

	Test() : FDApplication("Title", 1000, 600) {  }



};
