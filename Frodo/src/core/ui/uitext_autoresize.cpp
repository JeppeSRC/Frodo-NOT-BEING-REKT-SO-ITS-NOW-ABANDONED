#include "uitext.h"
#include "uiitem.h"

namespace FD {

void UITextAutoResize::RecalculateScale(FD_SCALE_MODE mode) {
	if (position != lastPosition) {
		orgPosition = position;
		FD_DEBUG("GG");
	}

	vec2 titleSize = font->GetFontMetrics(text);

	titleSize += margin;

	vec2 difference = adjustSize / titleSize;

	float finalScale = 1.0f;

	switch (mode) {
	case FD_SCALE_XY:
		finalScale = MIN(difference.x, difference.y);
		break;
	case FD_SCALE_ONLY_X:
		finalScale = difference.x;
		break;
	case FD_SCALE_ONLY_Y:
		finalScale = difference.y;
		break;
	}

	this->scale = vec2(finalScale, finalScale);

	titleSize += margin;
	titleSize *= scale;

	switch (alignment) {
	case FD_TEXT_ALIGN_CENTER:
		titleSize.x = 0;
		position = orgPosition - titleSize * 0.5f;
		break;
	case FD_TEXT_ALIGN_LEFT:
		titleSize.x = 0;
		titleSize.y = ((parent != nullptr ? parent->GetSize().y : 0.0f) * 0.5f) - (titleSize.y * 0.5f);
		position = orgPosition - titleSize * 0.5f;
		break;
	case FD_TEXT_ALIGN_RIGHT:
		position = orgPosition - titleSize;
		break;
	}
	
	lastPosition = position;
}


UITextAutoResize::UITextAutoResize(const String& name, vec2 position, vec2 adjustSize, Font* font, const String& text) : UIText(name, position, font, text) {
	this->adjustSize = adjustSize;
	orgPosition = position;
	lastPosition = position;
	RecalculateScale(FD_SCALE_XY);
}

void UITextAutoResize::SetMargin(const vec2& margin) {
	this->margin = margin;
	RecalculateScale(FD_SCALE_XY);
}

void UITextAutoResize::SetAdjustSize(const vec2& size) {
	this->adjustSize = size;
	RecalculateScale(FD_SCALE_XY);
}

void UITextAutoResize::SetText(const String& text) {
	this->text = text;
	RecalculateScale(FD_SCALE_XY);
}

void UITextAutoResize::SetTextAlignment(FD_TEXT_ALIGNMENT align) {
	this->alignment = align;
	RecalculateScale(FD_SCALE_XY);
}

}