#include "uiitem.h"

namespace FD {


void UITextAutoResize::RecalculateScale() {
	if (position != lastPosition) {
		orgPosition = position;
	}

	vec2 titleSize = font->GetFontMetrics(text);

	titleSize += margin;

	vec2 difference = adjustSize / titleSize;

	float finalScale = MIN(difference.x, difference.y);

	this->scale = vec2(finalScale, finalScale);

	titleSize += margin;
	titleSize *= scale;

	titleSize.x = 0;

	position = orgPosition - titleSize * 0.5f;
	lastPosition = position;
}


UITextAutoResize::UITextAutoResize(const String& name, vec2 position, vec2 adjustSize, Font* font, const String& text) : UIText(name, position, font, text) {
	this->adjustSize = adjustSize;
	orgPosition = position;
	lastPosition = position;
	RecalculateScale();
}

void UITextAutoResize::SetMargin(const vec2& margin) {
	this->margin = margin;
	RecalculateScale();
}

void UITextAutoResize::SetAdjustSize(const vec2& size) {
	this->adjustSize = size;
	RecalculateScale();
}

void UITextAutoResize::SetText(const String& text) {
	this->text = text;
	RecalculateScale();
}

}