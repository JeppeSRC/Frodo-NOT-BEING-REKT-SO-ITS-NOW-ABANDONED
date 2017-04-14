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

	size = vec2(2, text->adjustSize.y - (text->margin.y * 0.5f) + 10);
	position.y = (text->adjustSize.y * 0.5f) - (size.y * 0.5f);

	float length = text->GetFont()->GetFontMetrics(text->text, text->scale).x;

	position.x = text->position.x + length;	
}

}