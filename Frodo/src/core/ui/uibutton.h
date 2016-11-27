#pragma once

#include <fd.h>
#include "uiitem.h"

class FDAPI UIButton : public UIItem {
public:
	UIButton(const String& title, vec2 position, vec2 size);
	~UIButton() {}
};
