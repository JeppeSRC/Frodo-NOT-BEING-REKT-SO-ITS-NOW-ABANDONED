#include "uislider.h"

UISlider::UISlider(const String& name, vec2 position, vec2 size, ValueAction* action) : UIItem(position, size, name), action(action) {
	SetInteractable(true);
	SetVisible(true);
	SetAutoTextAdjustment(false);
	SetTitle("");
	sliderMargin = vec2(20, 0);

	float sliderPos = ((size.x - (sliderMargin.x * 2)) * action->GetInterpolation()) + sliderMargin.x;

	handle = new UISlider_Handle(vec2(sliderPos, (size.y / 2.0f) - 15), vec2(10, 30));
	handle->SetAutoTextAdjustment(false);
	handle->SetParent(this);
	handle->SetColor(vec4(1, 0, 1, 1));

}

void UISlider::OnEntered() {
	if (mouseOnTexture != nullptr) activeTexture = mouseOnTexture;
}

void UISlider::OnLeft() {
	activeTexture = defaultTexture;
	grabbed = -1.0f;
}

void UISlider::OnPressed(vec2 position) {
	if (pressedTexture != nullptr) activeTexture = pressedTexture;

	vec2& hPos = handle->GetPosition();
	vec2& hSize = handle->GetSize();

	if (position.x > hPos.x && position.x < hPos.x + hSize.x) {
		if (position.y > hPos.y && position.y < hPos.y + hSize.y) {
			grabbed = position.x - hPos.x;
		}
	}
}

void UISlider::OnReleased() {
	activeTexture = defaultTexture;
	grabbed = -1.0f;
}

void UISlider::OnDrag(vec2 position) {
	if (grabbed >= 0) {
		position.x -= grabbed;
		if (position.x < sliderMargin.x) position.x = sliderMargin.x;
		else if (position.x + grabbed + handle->GetSize().x > size.x - sliderMargin.x) position.x = size.x - sliderMargin.x - handle->GetSize().x;

		position.y = size.y / 2.0f - handle->GetSize().y / 2.0f;
		handle->SetPosition(position);

		action->Interpolate((position.x - sliderMargin.x) / (size.x - sliderMargin.x * 2 - handle->GetSize().x));
	}
}

void UISlider::OnAdd() {
	handler->Add(handle);
}

