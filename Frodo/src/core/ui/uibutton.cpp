#include "uibutton.h"
#include <core/log.h>
#include <graphics/texture/texturemanager.h>

namespace FD {

UIButton::UIButton(const String& name, vec2 position, vec2 size, const String& text) : UIItem(name, position, size) {

	SetColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SetTexture(nullptr);
	SetInteractable(true);
	SetVisible(true);

	UITextAutoResize* uitext = new UITextAutoResize("title", size * 0.5f, size, Font::GetDefaultFont(), text);
	uitext->SetParent(this);
	uitext->SetTextAlignment(FD_TEXT_ALIGN_CENTER);

	texts.Push_back(uitext);
}

void UIButton::OnEntered() {
	FD_DEBUG("[UIItem] OnEntered button \"%s\"", *name);
}

void UIButton::OnLeft() {
	FD_DEBUG("[UIItem] OnLeft button \"%s\"", *name);
}

void UIButton::OnPressed(vec2 position) {
	FD_DEBUG("[UIItem] OnPressed button \"%s\" at vec2(%f, %f)", *name, position.x, position.y);
}

void UIButton::OnReleased() {
	FD_DEBUG("[UIItem] OnReleased button \"%s\"", *name);
}

void UIButton::OnAdd() {
	
}

}