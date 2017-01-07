#include "uislider.h"

UISlider::UISlider_Handle::UISlider_Handle(vec2 position, vec2 size) : UIItem(position, size, "") {
	SetVisible(true);
	SetInteractable(false);
}

