#pragma once

#include "uiitem.h"
#include <core/event/eventlistener.h>

namespace FD {

class UITextBox : public UIItem {
protected:
	String text;

public:
	UITextBox(const String& name, vec2 position, vec2 size);

	
	void OnKey(uint32 key) override;

	inline String& GetText() { return text; }
};

}