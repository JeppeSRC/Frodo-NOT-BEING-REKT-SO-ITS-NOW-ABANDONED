#pragma once



#include "batchrenderer.h"
#include <math/vec4.h>
#include <graphics/texture/texture2d.h>

class FDAPI FontRenderer : public BatchRenderer {
protected:

public:
	FontRenderer(Window* window, uint32 max_glyphs);
	~FontRenderer();

	void SubmitText(const String& text, Font* font, vec2 position, vec4 color = vec4(1, 1, 1, 1));

	void Submit(Entity* e) { }
};
