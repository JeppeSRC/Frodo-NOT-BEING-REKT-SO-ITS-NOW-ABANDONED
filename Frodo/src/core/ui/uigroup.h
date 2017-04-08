#pragma once
#include "uiitem.h"

namespace  FD {

class FDAPI UIGroup : public UIItem {
private:

public:
	UIGroup(const String& name, vec2 position);

	void Add(UIItem* item);
};
}