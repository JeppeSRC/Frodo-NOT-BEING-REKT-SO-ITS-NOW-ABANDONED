#include "uitext.h"
#include "uiitem.h"

namespace FD {

void UITextHorizontalScroll::CalculateString() {
	text = orgText;
	RecalculateScale(FD_SCALE_ONLY_Y_ADD_MARGIN);

	vec2 size = font->GetFontMetrics(text, scale);

	if (size.x > adjustSize.x - margin.x) {
		float averageCharSize = size.x / text.length;

		uint32 charSlots = (uint32)((adjustSize.x - margin.x) / averageCharSize);

		uint32 totalOffset = charSlots;


		cursor->offset = orgText.length - totalOffset;
		text = orgText.SubString(cursor->offset, orgText.length);

		bool sizeAdjusted = false;

		while (true) {
			size = font->GetFontMetrics(text, scale);

			if (size.x < adjustSize.x - margin.x) {
				sizeAdjusted = true;
				cursor->offset = orgText.length - ++charSlots;
				text = orgText.SubString(cursor->offset, orgText.length);
			} else if (sizeAdjusted) {
				text.Remove(0, 1);
				cursor->offset--;
				break;
			} else {
				break;
			}
		}

		RecalculateScale(FD_SCALE_ONLY_Y_ADD_MARGIN);
	}
}

UITextHorizontalScroll::UITextHorizontalScroll(const String& name, vec2 position, vec2 adjustSize, Font* font) : UITextAutoResize(name, position, adjustSize, font, "") {
	cursor = new UITextHorizontalScroll_Cursor(name, 0, vec2(5, adjustSize.y - margin.y), this);
}

void UITextHorizontalScroll::SetText(const String& text) {
	orgText = text;
	CalculateString();
}

void UITextHorizontalScroll::Append(const String& text) {
	orgText += text;
	cursor->location += text.length;
	CalculateString();
}

void UITextHorizontalScroll::Append(const char character) {
	orgText += character;
	cursor->location++;
	CalculateString();
}

void UITextHorizontalScroll::Remove(const String& text) {
	orgText.Remove(text);
	cursor->location -= text.length;
	CalculateString();
}

void UITextHorizontalScroll::Remove(uint_t start, uint_t end) {
	orgText.Remove(start, end);
	cursor->location -= end - start;
	CalculateString();
}

void UITextHorizontalScroll::Remove() {
	if (orgText.length != 0) {
		orgText.Remove(orgText.length - 1, orgText.length);
		if (cursor->offset == 0) cursor->location--;
	}
	CalculateString();
}

void UITextHorizontalScroll::EnableCursor() {
	if (parent) {
		parent->GetHandler()->Add(cursor);
		cursor->SetParent(parent);
	}
	else {
		FD_WARNING("[UITextHorizontalScroll] Can't enable cursor, parent null in \"%s\"", *name);
	}
}

void UITextHorizontalScroll::DisableCursor() {
	parent->GetHandler()->Remove(cursor);
}
}
