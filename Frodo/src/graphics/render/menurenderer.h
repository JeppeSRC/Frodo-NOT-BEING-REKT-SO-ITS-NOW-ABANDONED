#pragma once

#include "fontrenderer.h"

#include <core/ui/uiitem.h>

class FDAPI MenuRenderer : public BatchRenderer {
private:
	FontRenderer* fontRenderer;

public:
	MenuRenderer(Window* window, unsigned int max_items);
	~MenuRenderer();

	void Begin() override;
	void Submit(const UIHandler* menu);
	void End() override;
	void Render() override;
};
