#include "uitext.h"

namespace FD {

UITextHorizontalScroll::UITextHorizontalScroll_Cursor::UITextHorizontalScroll_Cursor(const String& name, uint32 location, vec2 size, UITextHorizontalScroll* text) : UIItem(name + "_Cursor", vec2(0, 0), size) {
	this->location = location;
	this->text = text;

	timer = 0;

	SetColor(vec4(1.0f, 1.0f, 1.0f, 1));
	SetTexture(nullptr);
	SetVisible(true);
	SetInteractable(false);
}

void UITextHorizontalScroll::UITextHorizontalScroll_Cursor::Update(float delta) {

	timer += delta;

	if (timer >= 0.5f) {
		timer = 0;
		SetVisible(!isVisible);
	}

	const UITextHorizontalScroll& text = *this->text;

	size = vec2(2, text.font->GetSize() * text.scale.y);
	position.y = (text.adjustSize.y * 0.5f) - (size.y * 0.5f);

	CalculateCursurPosition();
}

void UITextHorizontalScroll::UITextHorizontalScroll_Cursor::CalculateCursurPosition() {
	const UITextHorizontalScroll& text = *this->text;

	if (location == 0) {
		position.x = text.position.x + text.position.x;
		return;
	} 

	if (location >= text.text.length) {
		location = (uint32)text.text.length;
		position.x = text.font->GetFontMetrics(text.text, text.scale).x + text.position.x;
		return;
	}

	String substr = text.text.SubString(0, location);

	position.x = text.font->GetFontMetrics(substr, text.scale).x + text.position.x;
}
}