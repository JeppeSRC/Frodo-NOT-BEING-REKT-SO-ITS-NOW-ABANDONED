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

class FDAPI UIText {
public:
	typedef vec2(*RESIZE_FUNCTION)(const vec2& size);
private:
	friend class UIItem;
protected:
	UIItem* parent;
	String name;

	String text;
	Font* font;

	vec2 position;
	vec2 offset;
	vec2 scale;
	vec4 color;
	
	FD_TEXT_ALIGNMENT alignment;

	RESIZE_FUNCTION adjustPosition;

public:
	UIText(const String& name, vec2 position, Font* font, const String& text) : name(name), text(text), font(font), position(position), scale(vec2(1, 1)), color(vec4(1, 1, 1, 1)) {}

	inline UIItem* GetParent() const { return parent; }
	inline const String& GetName() const { return name; }
	inline const String& GetText() const { return text; }
	inline Font* GetFont() const { return font; }
	inline vec2 GetPosition() const { return position; }
	inline vec2 GetOffset() const { return offset; }
	inline vec2 GetScale() const { return scale; }
	inline vec4 GetColor() const { return color; }
	inline FD_TEXT_ALIGNMENT GetTextAlignment() const { return alignment; }

	inline void SetParent(UIItem* parent) { this->parent = parent; }
	inline void SetName(const String& name) { this->name = name; }
	virtual void SetText(const String& name) { this->text = text; }
	inline void SetFont(Font* font) { this->font = font; }
	inline void SetPosition(const vec2& position) { this->position = position; }
	inline void SetOffset(const vec2& offset) { this->offset = offset; }
	inline void SetScale(const vec2& scale) { this->scale = scale; }
	inline void SetColor(const vec4& color) { this->color = color; }
	inline void SetTextAlignment(FD_TEXT_ALIGNMENT alignment) { this->alignment = alignment; }
};



class FDAPI UITextAutoResize : public UIText {
private:
	void RecalculateScale();

	vec2 orgPosition;
	vec2 lastPosition;
protected:
	vec2 margin;
	vec2 adjustSize;

public:
	UITextAutoResize(const String& name, vec2 position, vec2 adjustSize, Font* font, const String& text);

	inline vec2 GetAdjustSize() const { return adjustSize; }

	void SetMargin(const vec2& margin);
	void SetAdjustSize(const vec2& size);
	void SetText(const String& text) override;
};

class FDAPI UIItem {
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

	inline UIText* GetText(const String& name) { 
		return texts[texts.Find<const String&>([](UIText* item, const String& name) -> bool { return item->GetName() == name; }, name)];
	}
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

	inline void SetFont(Font* font) {
		for (uint_t i = 0; i < texts.GetSize(); i++)
			texts[i]->SetFont(font);
	}

	inline void SetFont(Font* font, uint_t index) {
		if (texts.GetSize() >= index + 1) {
			texts[index]->SetFont(font);
		}
	}
};

}