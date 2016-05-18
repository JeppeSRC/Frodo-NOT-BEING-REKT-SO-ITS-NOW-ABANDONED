#pragma once

#include <frodo.h>

class Test2D : public FDApplication {
protected:

	void OnInit() override;
	void OnTick() override;
	void OnUpdate(float delta) override;
	void OnRender() override;
	void OnExit() override;

private:
	Renderer2D* renderer;
	List<Renderable2D*> renderables;

	unsigned int fps;

public:
	Test2D();

};

