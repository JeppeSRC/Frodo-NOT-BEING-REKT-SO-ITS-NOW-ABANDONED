#pragma once

#include <fd.h>
#include "uiitem.h"

namespace FD {

class FDAPI UIButton : public UIItem {
private:
	UITextAutoResize* text;
public:
	UIButton(const String& title, vec2 position, vec2 size);
	~UIButton() { delete text; }

	virtual void OnEntered() override;
	virtual void OnLeft() override;
	virtual void OnPressed(vec2 position) override;
	virtual void OnReleased() override;
	virtual void OnAdd() override;
};

}