#include "ui.h"
#include <core/event/fd_key.h>
#include <core/input.h>

namespace FD {

UITextBox::UITextBox(const String& name, vec2 position, vec2 size) : UIItem(name, position, size) {
	text = new UITextHorizontalScroll("content", vec2(0, 0), size, Font::GetDefaultFont());
	text->SetTextAlignment(FD_TEXT_ALIGN_LEFT);
	text->SetParent(this);

	texts.Push_back(text);

	SetTexture(nullptr);
	SetVisible(true);
	SetInteractable(true);
}

void UITextBox::OnKey(uint32 key) {
	if (key == FD_KEY_BACK) {
		text->Remove();
	} else {
		uint32 c = KeyMap::GetChar((FD_KEY)key, Input::GetModifiers());
		if (text->GetFont()->IsCharLoaded(c)) {
			text->Append((const char)c);
		}
	}

	
}

void UITextBox::OnFocus() {
	text->EnableCursor();
}

void UITextBox::OnFocusLost() {
	text->DisableCursor();
}

}