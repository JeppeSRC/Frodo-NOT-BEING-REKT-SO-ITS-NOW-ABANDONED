#include "uibutton.h"

UIButton::UIButton(const String& title, vec2 position, vec2 size) : UIItem(position, size, title) {
	
	SetColor(vec4(1.0f, 0.0f, 1.0f, 0.125f));
	SetTexture(nullptr);
	SetInteractable(true);
	SetVisible(true);
	SetTitleMargin(vec2(50, 50));
	SetTitleOffset(vec2(0, -5));
	SetTitleColor(vec4(1, 1, 1, 1.0f));
}