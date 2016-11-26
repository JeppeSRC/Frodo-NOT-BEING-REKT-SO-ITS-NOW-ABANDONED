#pragma once

#include "fontrenderer.h"

#include <core/ui/menuitem.h>

class FDAPI MenuRenderer : public BatchRenderer {
private:
	FontRenderer* fontRenderer;

public:
	MenuRenderer(Window* window, unsigned int max_items);
	~MenuRenderer();

	void Submit(const Menu* menu);
};
