#include "uibutton.h"
#include <core/log.h>
#include <graphics/texture/texturemanager.h>

namespace FD {

UIButton::UIButton(const String& title, vec2 position, vec2 size) : UIItem(position, size, title) {

	SetColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SetTexture(nullptr);
	SetInteractable(true);
	SetVisible(true);

	text = new UITextAutoResize(title, Font::GetDefaultFont(), vec2(0, 0), vec4(1, 0, 1, 1), size);
	text->SetParent(this);
	text->SetMargin(vec2(5, 5));
	text->SetTextAlignment(FD_TEXT_ALIGN_CENTER);
	text->SetPosition([](const vec2& size) -> vec2 { return vec2(size.x / 2.0f, 0); });
	text->UpdatePosition();

	texts.Push_back(text);
}

void UIButton::OnEntered() {
	FD_DEBUG("[UIItem] OnEntered button \"%s\"", *texts[0]->GetText());
}

void UIButton::OnLeft() {
	FD_DEBUG("[UIItem] OnLeft button \"%s\"", *texts[0]->GetText());
}

void UIButton::OnPressed(vec2 position) {
	FD_DEBUG("[UIItem] OnPressed button \"%s\" at vec2(%f, %f)", *texts[0]->GetText(), position.x, position.y);
}

void UIButton::OnReleased() {
	FD_DEBUG("[UIItem] OnReleased button \"%s\"", *texts[0]->GetText());
}

void UIButton::OnAdd() {
	
}

}