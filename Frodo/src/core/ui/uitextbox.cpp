#include "uitextbox.h"

namespace FD {

UITextBox::UITextBox(const String& name, vec2 position, vec2 size) : UIItem(name, position, size) {
	text = new UITextHorizontalScroll("content", vec2(0, 0), size, Font::GetDefaultFont());
	text->SetParent(this);
	text->SetTextAlignment(FD_TEXT_ALIGN_LEFT);
	text->SetMargin(vec2(10, 10));
//	text->SetOffset(vec2(0, -10));

	texts.Push_back(text);

	SetColor(vec4(1, 0.5f, 0, 1));
	SetTexture(nullptr);
	SetVisible(true);
	SetInteractable(true);
}

void UITextBox::OnKey(uint32 key) {
	text->Append((char)key);
}

}