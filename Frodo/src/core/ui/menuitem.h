#pragma once

#include <fd.h>
#include <math/vec2.h>
#include <math/vec4.h>
#include <util/string.h>
#include <graphics/texture/texture2d.h>
#include "menu.h"

class MenuItem {
protected:
	String title;
	String description;

	ivec2 position;
	ivec2 size;

	bool interactable = true;
	bool visible = true;

	MenuItem* parent;
	Menu* menu;

	vec4 color;
	Texture2D* texture;

	MenuItem(ivec2 position, ivec2 size, const String& title) : position(position), size(size), title(title) { }
public:
	virtual ~MenuItem() { }

	virtual void OnClick(ivec2 position) { }
	virtual void OnPressed(ivec2 position) { }
	virtual void OnDrag(ivec2 position) { }
	virtual void OnReleased(ivec2 position) { }
	virtual void OnHover(ivec2 position) { }
	virtual void OnEntered() { }
	virtual void OnLeft() { }

	virtual void Update() { }

	inline ivec2 GetPosition() const { return position; }
	inline ivec2 GetSize() const { return size; }
	inline String GetTitle() const { return title; }
	inline String GetDescription() const { return description; }
	inline MenuItem* GetParent() const { return parent; }
	inline Menu* GetMenu() const { return menu; }
	inline Texture2D* GetTexture() const { return texture; }
	inline vec4 GetColor() const { return color; }
	inline bool IsInteractable() const { return interactable; }
	inline bool IsVisible() const { return visible; }

	inline void SetMenu(Menu* menu) { this->menu = menu; }
	inline void SetPosition(ivec2 position) { this->position = position; }
	inline void SetSize(ivec2 size) { this->size = size; }
	inline void SetTitle(const String& title) { this->title = title; }
	inline void SetDescription(const String& description) { this->description = description; }
	inline void SetParent(MenuItem* parent) { this->parent = parent; }
	inline void SetTexture(Texture2D* texture) { this->texture = texture; }
	inline void SetColor(vec4 color) { this->color = color; }
	inline bool SetInteractable(bool interactable) { this->interactable = interactable; }
	inline bool SetVisible(bool visible) { this->visible = visible; }
};
