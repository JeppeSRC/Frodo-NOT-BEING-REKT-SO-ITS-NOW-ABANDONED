#pragma once

#include <fd.h>
#include <math/vec2.h>
#include <math/vec4.h>
#include <util/string.h>
#include <graphics/texture/texture2d.h>
#include "uihandler.h"

class UIItem {
protected:
	String title;
	vec2 titleOffset;
	vec4 titleColor;

	String description;
	vec4 descriptionColor;

	vec2 position;
	vec2 size;
	vec2 titleMargin;

	bool interactable = true;
	bool visible = true;

	UIItem* parent;
	UIHandler* handler;

	vec4 color;
	Texture2D* texture;

	UIItem(vec2 position, vec2 size, const String& title) : position(position), size(size), title(title) { }
public:
	virtual ~UIItem() { }

	virtual void OnClick(ivec2 position) { }
	virtual void OnPressed(ivec2 position) { }
	virtual void OnDrag(ivec2 position) { }
	virtual void OnReleased(ivec2 position) { }
	virtual void OnHover(ivec2 position) { }
	virtual void OnEntered() { }
	virtual void OnLeft() { }

	virtual void Update() { }

	inline UIHandler* GetHandler() const { return handler; }
	inline vec2 GetPosition() const { return position; }
	inline vec2 GetSize() const { return size; }

	inline vec2 GetTitleMargin() const { return titleMargin; }
	inline vec2 GetTitleOffset() const { return titleOffset; }
	inline vec4 GetTitleColor() const { return titleColor; }
	inline String GetTitle() const { return title; }

	inline String GetDescription() const { return description; }
	inline UIItem* GetParent() const { return parent; }
	inline Texture2D* GetTexture() const { return texture; }
	inline vec4 GetColor() const { return color; }
	inline bool IsInteractable() const { return interactable; }
	inline bool IsVisible() const { return visible; }


	inline void SetHandler(UIHandler* handler) { this->handler = handler; }
	inline void SetPosition(vec2 position) { this->position = position; }
	inline void SetSize(vec2 size) { this->size = size; }

	inline void SetTitleMargin(vec2 margin) { this->titleMargin = margin; }
	inline void SetTitleOffset(vec2 offset) { this->titleOffset = offset; }
	inline void SetTitleColor(vec4 color) { this->titleColor = color; }
	inline void SetTitle(const String& title) { this->title = title; }

	inline void SetDescription(const String& description) { this->description = description; }
	inline void SetParent(UIItem* parent) { this->parent = parent; }
	inline void SetTexture(Texture2D* texture) { this->texture = texture; }
	inline void SetColor(vec4 color) { this->color = color; }
	inline void SetInteractable(bool interactable) { this->interactable = interactable; }
	inline void SetVisible(bool visible) { this->visible = visible; }
};
