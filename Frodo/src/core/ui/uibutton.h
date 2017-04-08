#pragma once

#include <fd.h>
#include "uiitem.h"

namespace FD {

class FDAPI UIButton : public UIItem {
public:
	UIButton(const String& name, vec2 position, vec2 size, const String& text);
	~UIButton() { }

	virtual void OnEntered() override;
	virtual void OnLeft() override;
	virtual void OnPressed(vec2 position) override;
	virtual void OnReleased() override;
	virtual void OnAdd() override;
};

}