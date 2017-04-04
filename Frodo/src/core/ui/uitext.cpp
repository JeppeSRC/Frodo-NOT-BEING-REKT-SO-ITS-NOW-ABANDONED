#include "uiitem.h"

namespace FD {

/*	vec2 titleSize = font->GetFontMetrics(title);

	titleSize += margin;
	
	vec2 difference = size / titleSize;
	
	float finalScale = MIN(difference.x, difference.y);
	
	vec2 scale(finalScale, finalScale);
	
	titleSize += margin * 0.5f;
	titleSize *= scale;
	
	fontRenderer->SubmitText(title, font, position + (size / 2.0f) - (titleSize / 2.0f) + item->GetTitleOffset() * scale, item->GetTitleColor(), scale, FD_TEXT_ALIGN_LEFT);*/

void UITextAutoResize::UpdateProperties() {
	vec2 titleSize = font->GetFontMetrics(text);

	titleSize += margin;

	vec2 difference = adjustSize / titleSize;

	float finalScale = MIN(difference.x, difference.y);
	
	this->scale = vec2(finalScale, finalScale);
}


UITextAutoResize::UITextAutoResize(const String& text, Font* font, vec2 position, vec4 color, vec2 adjustSize) : UIText(text, font, position, vec2(1, 1), color) {
	this->adjustSize = adjustSize;
	UpdateProperties();
}

void UITextAutoResize::SetPosition(const vec2& position) {
	this->position = position;
	UpdateProperties();
}

void UITextAutoResize::SetMargin(const vec2& margin) {
	this->margin = margin;
	UpdateProperties();
}

void UITextAutoResize::SetAdjustSize(const vec2& size) {
	this->adjustSize = size;
	UpdateProperties();
}

void UITextAutoResize::UpdatePosition() {
	this->position = adjustPosition(parent->GetSize());
}

}