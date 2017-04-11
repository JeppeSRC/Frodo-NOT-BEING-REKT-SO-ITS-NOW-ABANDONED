#pragma once

#include "uiitem.h"
#include "uitext.h"
#include <core/event/eventlistener.h>

namespace FD {

class FDAPI UITextBox : public UIItem {
protected:
	UITextHorizontalScroll* text;

public:
	UITextBox(const String& name, vec2 position, vec2 size);

	void OnKey(uint32 key) override;

	inline void SetText(const String& text) { this->text->SetText(text); }

	inline const String& GetText() const { return text->GetText(); }
};

}