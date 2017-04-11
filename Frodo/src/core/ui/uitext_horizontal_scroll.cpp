#include "uitext.h"
#include "uiitem.h"

namespace FD {

void UITextHorizontalScroll::CalculateString() {
	text = orgText;
	RecalculateScale(FD_SCALE_ONLY_Y);

	vec2 size = font->GetFontMetrics(text, scale);

	if (size.x > adjustSize.x) {
		float averageCharSize = size.x / text.length;

		uint32 charSlots = (uint32)(adjustSize.x / averageCharSize);

		text = orgText.SubString(orgText.length - charSlots, orgText.length);
	}
}

UITextHorizontalScroll::UITextHorizontalScroll(const String& name, vec2 position, vec2 adjustSize, Font* font) : UITextAutoResize(name, position, adjustSize, font, "") {
	
}

void UITextHorizontalScroll::SetText(const String& text) {
	orgText = text;
	CalculateString();
}

void UITextHorizontalScroll::Append(const String& text) {
	orgText += text;
	CalculateString();
}

void UITextHorizontalScroll::Append(const char character) {
	orgText += character;
	CalculateString();
}

}
