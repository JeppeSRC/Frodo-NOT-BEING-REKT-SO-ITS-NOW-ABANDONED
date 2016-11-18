#pragma once

#include <fd.h>
#include <math/vec2.h>
#include <util/string.h>
#include "menu.h"

class MenuItem {
protected:
	ivec2 position;
	ivec2 size;

	bool interacable = true;
	bool visible = true;

	MenuItem* parent;

	Menu* menu;

	String title;
	String description;

	MenuItem() {}
public:
	virtual ~MenuItem() { }

	virtual void OnClick(ivec2 position) {}
	virtual void OnPressed(ivec2 position) {}
	virtual void OnDrag(ivec2 position) {}
	virtual void OnReleased(ivec2 position) {}
	virtual void OnHover(ivec2 position) {}
	virtual void OnEntered() {}
	virtual void OnLeft() {}

	virtual void Update() {}

	inline ivec2 GetPosition() const { return position; }
	inline ivec2 GetSize() const { return size; }

	inline void SetMenu(Menu* menu) { this->menu = menu; }
};
