#include "uibutton.h"
#include <core/log.h>
#include <graphics/texture/texturemanager.h>

namespace FD {

UIButton::UIButton(const String& title, vec2 position, vec2 size) : UIItem(position, size, title) {

	SetColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SetTexture(nullptr);
	SetInteractable(true);
	SetVisible(true);
	SetTitleMargin(vec2(70, 50));
	SetTitleOffset(vec2(0, -5));
	SetTitleColor(vec4(1, 1, 1, 1.0f));
}

void UIButton::OnEntered() {
	FD_DEBUG("[UIItem] OnEntered button \"%s\"", *title);
}

void UIButton::OnLeft() {
	FD_DEBUG("[UIItem] OnLeft button \"%s\"", *title);
}

void UIButton::OnPressed(vec2 position) {
	FD_DEBUG("[UIItem] OnPressed button \"%s\" at vec2(%f, %f)", *title, position.x, position.y);
}

void UIButton::OnReleased() {
	FD_DEBUG("[UIItem] OnReleased button \"%s\"", *title);
}

}