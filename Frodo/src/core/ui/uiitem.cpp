#include "uiitem.h"
#include "uitext.h"

namespace FD {

UIText* UIItem::GetText(const String& name) {
	uint_t index = texts.Find<const String&>([](UIText* item, const String& name) -> bool { return item->GetName() == name; }, name);

	return index == (uint_t)-1 ? nullptr : texts[index];
}

void UIItem::SetFont(Font* font) {
	uint_t size = texts.GetSize();
	for (uint_t i = 0; i < size; i++)
		texts[i]->SetFont(font);
}

void UIItem::SetFont(Font* font, const String& name) {
	UIText* t = GetText(name);
	if (t) t->SetFont(font);
}

void UIItem::SetFont(Font* font, uint_t index) {
	if (index + 1 <= texts.GetSize())
		texts[index]->SetFont(font);
}

}