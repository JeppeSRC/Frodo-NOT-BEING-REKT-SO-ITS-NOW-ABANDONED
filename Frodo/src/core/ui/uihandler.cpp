#include "uihandler.h"
#include "uiitem.h"
#include <core/input.h>

UIHandler::UIHandler() {

}

UIHandler::~UIHandler() {

}

void UIHandler::Add(UIItem* item) {
	item->SetHandler(this);
	items.Push_back(item);
	item->OnAdd();
}

void UIHandler::Update() {
	size_t size = items.GetSize();
	for (size_t i = 0; i < size; i++) {
		items[i]->Update();
	}
}

bool UIHandler::OnMouseActionButtonPressed(unsigned int button) {
	size_t size = items.GetSize();
	for (size_t i = 0; i < size; i++) {
		UIItem* item = items[i];
		if (!item->IsMouseOnTop() || !item->IsInteractable()) continue;

		item->SetPressed(true);
		item->OnPressed(vec2((float)Input::mouseX, (float)Input::mouseY) - item->GetAbsolutePosition());
	}


	return false;
}

bool UIHandler::OnMouseActionButtonReleased(unsigned int button) {
	size_t size = items.GetSize();
	for (size_t i = 0; i < size; i++) {
		UIItem* item = items[i];
		if (!item->IsMouseOnTop() || !item->IsInteractable()) continue;

		item->SetPressed(false);
		item->OnReleased();
	}

	return false;
}

bool UIHandler::OnMouseActionMove(ivec2 position) {
	size_t size = items.GetSize();
	for (size_t i = 0; i < size; i++) {
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