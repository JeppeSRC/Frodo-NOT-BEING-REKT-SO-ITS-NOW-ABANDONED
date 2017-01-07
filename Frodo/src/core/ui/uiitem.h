#pragma once

#include <fd.h>
#include <math/vec2.h>
#include <math/vec4.h>
#include <util/string.h>
#include <graphics/texture/texture2d.h>
#include "uihandler.h"

class FDAPI UIItem {
protected:
	String title;
	vec2 titleOffset;
	vec4 titleColor = vec4(1, 1, 1, 1);
	vec2 titleScale = vec2(1, 1);
	vec2 titleMargin;

	String description;
	vec4 descriptionColor;

	vec2 position;
	vec2 size;

	bool isInteractable = false;
	bool isVisible = true;
	bool isMouseOnTop = false;
	bool isPressed = false;
	bool autoAdjustText = true;
	

	UIItem* parent = nullptr;
	UIHandler* handler = nullptr;

	vec4 color = vec4(1, 1, 1, 1);
	Texture2D* activeTexture = nullptr;
	Texture2D* defaultTexture = nullptr;
	Texture2D* mouseOnTexture = nullptr;
	Texture2D* pressedTexture = nullptr;

	UIItem(vec2 position, vec2 size, const String& title) : position(position), size(size), title(title) { }
public:
	virtual ~UIItem() { }

	virtual void OnClick() { }
	virtual void OnPressed(vec2 position) { }
	virtual void OnDrag(vec2 position) { }
	virtual void OnReleased() { }
	virtual void OnHover() { }
	virtual void OnEntered() { }
	virtual void OnLeft() { }
	virtual void OnAdd() {}

	virtual void Update() { }

	inline UIHandler* GetHandler() const { return handler; }
	inline vec2 GetPosition() const { return position; }
	inline vec2 GetAbsolutePosition() const { if (parent != nullptr) return parent->GetAbsolutePosition() + position; return position; }
	inline vec2 GetSize() const { return size; }

	inline vec2 GetTitleMargin() const { return titleMargin; }
	inline vec2 GetTitleOffset() const { return titleOffset; }
	inline vec4 GetTitleColor() const { return titleColor; }
	inline vec2 GetTtitleScale() const { return titleScale; }
	inline String GetTitle() const { return title; }

	inline String GetDescription() const { return description; }
	inline UIItem* GetParent() const { return parent; }
	inline Texture2D* GetTexture() const { return activeTexture; }
	inline vec4 GetColor() const { return color; }
	inline bool IsInteractable() const { return isInteractable; }
	inline bool IsVisible() const { return isVisible; }
	inline bool IsMouseOnTop() const { return isMouseOnTop; }
	inline bool IsPressed() const { return isPressed; }
	inline bool AutoTextAdjustment() const { return autoAdjustText; }

	inline void SetHandler(UIHandler* handler) { this->handler = handler; }
	inline void SetPosition(vec2 position) { this->position = position; }
	inline void SetSize(vec2 size) { this->size = size; }

	inline void SetTitleMargin(vec2 margin) { this->titleMargin = margin; }
	inline void SetTitleOffset(vec2 offset) { this->titleOffset = offset; }
	inline void SetTitleColor(vec4 color) { this->titleColor = color; }
	inline void SetTitleScale(vec2 scale) { this->titleScale = scale; }
	inline void SetTitle(const String& title) { this->title = title; }

	inline void SetDescription(const String& description) { this->description = description; }
	inline void SetParent(UIItem* parent) { this->parent = parent; }
	inline void SetTexture(Texture2D* texture) { this->activeTexture = texture; }
	inline void SetColor(vec4 color) { this->color = color; }
	inline void SetInteractable(bool interactable) { this->isInteractable = interactable; }
	inline void SetVisible(bool visible) { this->isVisible = visible; }
	inline void SetPressed(bool pressed) { this->isPressed = pressed; }
	inline void SetMouseOnTop(bool ontop) { this->isMouseOnTop = ontop; }
	inline void SetAutoTextAdjustment(bool enable) { this->autoAdjustText = enable; }
};
