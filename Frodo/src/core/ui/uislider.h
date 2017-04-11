#pragma once
#include "uiitem.h"
#include <math/valueinterpolation.h>

namespace FD {

class FDAPI UISlider : public UIItem {
private:
	class UISlider_Handle : public UIItem {
	public:
		UISlider_Handle(vec2 position, vec2 size);

	};
protected:
	ValueAction* action;
	UISlider_Handle* handle;

	vec2 sliderMargin = vec2(5, 5);
	float grabbed = -1;

public:
	UISlider(const String& name, vec2 position, vec2 size, ValueAction* action);

	virtual void OnEntered() override;
	virtual void OnLeft() override;
	virtual void OnPressed(vec2 position) override;
	virtual void OnReleased() override;
	virtual void OnDrag(vec2 position) override;
	virtual void OnAdd() override;
};


}