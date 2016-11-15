#pragma once



#include "batchrenderer.h"

class FDAPI FontRenderer : public BatchRenderer {
protected:

public:
	FontRenderer(Window* window, unsigned int max_glyphs);
	~FontRenderer();

	void SubmitText(const String& text, Font* font, vec2 position);
	void Submit(Entity* e) { }
};
