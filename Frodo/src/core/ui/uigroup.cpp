#include "uigroup.h"
#include <core/log.h>

UIGroup::UIGroup(vec2 position, const String& title) : UIItem(position, vec2(0, 0), title) {
	SetInteractable(false);
	SetVisible(false);
}

void UIGroup::Add(UIItem* item) {
	if (handler == nullptr) {
		FD_FATAL("[UIGroup] UIHandler in \"%s\" is not set. Add group to the handler before adding items to the group", *title);
		return;
	}

	item->SetParent(this);
	handler->Add(item);
}