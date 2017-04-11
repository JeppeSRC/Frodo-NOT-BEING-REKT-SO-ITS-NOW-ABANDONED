#pragma once

#include <fd.h>
#include <math/vec2.h>
#include <math/vec4.h>
#include <util/string.h>
#include <graphics/texture/texture2d.h>
#include <graphics/font/font.h>
#include <graphics/render/renderer/fontrenderer.h>
#include "uihandler.h"

namespace FD {

class FDAPI UIItem {
private:
	friend class UIText;
protected:
	String name;

	List<UIText*> texts;

	vec2 position;
	vec2 size;

	bool isInteractable = false;
	bool isVisible = true;
	bool isMouseOnTop = false;
	bool isPressed = false;

	UIItem* parent = nullptr;
	UIHandler* handler = nullptr;

	vec4 color = vec4(1, 1, 1, 1);
	Texture2D* texture;

	UIItem(const String& name, vec2 position, vec2 size) : position(position), size(size), name(name) { texture = nullptr; }
public:
	virtual ~UIItem() {}

	virtual void OnClick() {}
	virtual void OnPressed(vec2 position) {}
	virtual void OnDrag(vec2 position) {}
	virtual void OnReleased() {}
	virtual void OnHover() {}
	virtual void OnEntered() {}
	virtual void OnLeft() {}
	virtual void OnKey(uint32 key) {}
	virtual void OnAdd() {}

	virtual void Update() {}

	inline UIHandler* GetHandler() const { return handler; }
	inline vec2 GetPosition() const { return position; }
	inline vec2 GetAbsolutePosition() const { if (parent != nullptr) return parent->GetAbsolutePosition() + position; return position; }
	inline vec2 GetSize() const { return size; }
	inline const String& GetName() const { return name; }

	UIText* GetText(const String& name);

	inline List<UIText*>& GetTexts() { return texts; }
	inline void AddText(UIText* text) { texts.Push_back(text); }

	inline UIItem* GetParent() const { return parent; }
	inline Texture2D* GetTexture() const { return texture; }
	inline vec4 GetColor() const { return color; }
	inline bool IsInteractable() const { return isInteractable; }
	inline bool IsVisible() const { return isVisible; }
	inline bool IsMouseOnTop() const { return isMouseOnTop; }
	inline bool IsPressed() const { return isPressed; }

	inline void SetHandler(UIHandler* handler) { this->handler = handler; }
	inline void SetPosition(vec2 position) { this->position = position; }
	inline void SetSize(vec2 size) { this->size = size; }
	inline void SetName(const String& name) { this->name = name; }

	inline void SetParent(UIItem* parent) { this->parent = parent; }
	inline void SetTexture(Texture2D* texture) { this->texture = texture; }
	inline void SetColor(vec4 color) { this->color = color; }
	inline void SetInteractable(bool interactable) { this->isInteractable = interactable; }
	inline void SetVisible(bool visible) { this->isVisible = visible; }
	inline void SetPressed(bool pressed) { this->isPressed = pressed; }
	inline void SetMouseOnTop(bool ontop) { this->isMouseOnTop = ontop; }

	void SetFont(Font* font);
	void SetFont(Font* font, const String& name);
	void SetFont(Font* font, uint_t index);

};

}