#pragma once

#include "fontrenderer.h"

#include <core/ui/uiitem.h>

namespace FD {

class FDAPI MenuRenderer : public BatchRenderer {
private:
	FontRenderer* fontRenderer;

public:
	MenuRenderer(Window* window, unsigned int max_items);
	~MenuRenderer();

	void Begin(Camera* camera) override;
	void Submit(const UIHandler* menu);
	void End() override;
	void Present() override;
};

}