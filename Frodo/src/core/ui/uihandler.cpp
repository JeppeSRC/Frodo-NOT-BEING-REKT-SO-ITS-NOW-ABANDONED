#include "uihandler.h"
#include "uiitem.h"
#include <core/input.h>


namespace FD {

UIHandler::UIHandler() {
	inFocus = nullptr;
}

UIHandler::~UIHandler() {

}

void UIHandler::Add(UIItem* item) {
	item->SetHandler(this);
	items.Push_back(item);
	item->OnAdd();
}

void UIHandler::Remove(UIItem* item) {
	items.Remove(item);
}

void UIHandler::Update(float delta) {
	uint_t size = items.GetSize();
	for (uint_t i = 0; i < size; i++) {
		items[i]->Update(delta);
	}
}

bool UIHandler::OnMouseActionButtonPressed(unsigned int button) {
	uint_t size = items.GetSize();

	bool inFocusSet = false;
	for (uint_t i = 0; i < size; i++) {
		UIItem* item = items[i];
		if (!item->IsMouseOnTop() || !item->IsInteractable()) continue;

		item->SetPressed(true);
		//TODO: mouse pos
		item->OnPressed(vec2((float)0, (float)0) - item->GetAbsolutePosition());

		if (inFocus == item) goto skip_on_focus;
		
		if (inFocus) inFocus->OnFocusLost();
		inFocus = item;
		inFocus->OnFocus();

		skip_on_focus:
		inFocusSet = true;
	}

	if (!inFocusSet && inFocus) {
		inFocus->OnFocusLost();
		inFocus = nullptr;
	}

	return false;
}

bool UIHandler::OnMouseActionButtonReleased(unsigned int button) {
	uint_t size = items.GetSize();
	for (uint_t i = 0; i < size; i++) {
		UIItem* item = items[i];
		if (!item->IsMouseOnTop() || !item->IsInteractable()) continue;

		item->SetPressed(false);
		item->OnReleased();
	}

	return false;
}

bool UIHandler::OnMouseActionMove(ivec2 position) {
	uint_t size = items.GetSize();
	for (uint_t i = 0; i < size; i++) {
		UIItem* item = items[i];

		if (!item->IsInteractable()) continue;

		vec2 mousePos((float)position.x, (float)position.y);
		vec2 pos = mousePos - item->GetAbsolutePosition();
		vec2 size = item->GetSize();
		if ((pos.x < size.x && pos.y < size.y) && (pos.x >= 0 && pos.y >= 0)) {
			if (!item->IsMouseOnTop()) {
				item->OnEntered();
				item->SetMouseOnTop(true);
			} else {
				if (item->IsPressed()) item->OnDrag(pos);
			}
		} else {
			if (item->IsMouseOnTop()) {
				item->OnLeft();
				item->SetMouseOnTop(false);
			}
		}
	}

	return false;
}

bool UIHandler::OnKeyboardActionKeyPressed(uint32 key) {
	if (inFocus) inFocus->OnKey(key);
	return false;
}
}