#pragma once
#include "uiitem.h"

namespace  FD {

class FDAPI UIGroup : public UIItem {
private:

public:
	UIGroup(vec2 position, const String& title);

	void Add(UIItem* item);
};
}