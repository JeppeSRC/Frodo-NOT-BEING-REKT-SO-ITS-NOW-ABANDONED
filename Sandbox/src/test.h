#pragma once
#include <frodo.h>

class Test : public FDApplication {
protected:

	void OnInit() override;
	void OnTick() override;
	void OnUpdate(float delta) override;
	void OnRender() override;
	void OnExit() override;

public:
	FontRenderer* renderer;
	Font* font;

	Test() : FDApplication("Title", 1000, 600) {  }



};
