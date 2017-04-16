#pragma once
#include "batchrenderer.h"
#include <math/vec4.h>
#include <graphics/texture/texture2d.h>

namespace FD {

enum FD_TEXT_ALIGNMENT {
	FD_TEXT_ALIGN_CENTER,
	FD_TEXT_ALIGN_LEFT,
	FD_TEXT_ALIGN_RIGHT
};

class FDAPI FontRenderer : public BatchRenderer {
private:


public:
	FontRenderer(Window* window, uint32 max_glyphs);
	~FontRenderer();

	inline void SubmitText(const String& text, Font* font, vec2 position, vec4 color, vec2 scale, FD_TEXT_ALIGNMENT alignment) {
		switch (alignment) {
		case FD_TEXT_ALIGN_LEFT:
			SubmitTextAlignLeft(text, font, position, color, scale);
			break;
		case FD_TEXT_ALIGN_RIGHT:
			SubmitTextAlignRight(text, font, position, color, scale);
			break;
		case FD_TEXT_ALIGN_CENTER:
			SubmitTextAlignCenter(text, font, position, color, scale);
			break;
		}
	}

	void SubmitTextAlignLeft(const String& text, Font* font, vec2 position, vec4 color, vec2 scale);
	void SubmitTextAlignRight(const String& text, Font* font, vec2 position, vec4 color, vec2 scale);
	void SubmitTextAlignCenter(const String& text, Font* font, vec2 position, vec4 color, vec2 scale);

	void Submit(Entity3D* e) { }
};

}