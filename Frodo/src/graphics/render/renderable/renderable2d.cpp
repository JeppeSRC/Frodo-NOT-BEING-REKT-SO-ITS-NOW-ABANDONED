#include "renderable2d.h"

Renderable2D::Renderable2D(const vec2& position, const vec2& size, float rotation, unsigned int color, Texture2D* texture) {
	this->position = position;
	this->size = size;
	this->rotation = rotation;
	this->color = color;
	this->texture = texture;

}

Renderable2D::Renderable2D(const vec2& position, const vec2& size, unsigned int color, Texture2D* texture) : Renderable2D(position, size, 0, color, texture) { }
Renderable2D::Renderable2D(const vec2& position, const vec2& size, unsigned int color) : Renderable2D(position, size, color, nullptr) { }
Renderable2D::Renderable2D(const vec2& position, const vec2& size, Texture2D* texture) : Renderable2D(position, size, 0xFFFFFFFF, texture) { }

Renderable2D::Renderable2D(const vec2& position, unsigned int color, Texture2D* texture) : Renderable2D(position, vec2(1, 1), color, texture) { }
Renderable2D::Renderable2D(const vec2& position, unsigned int color) : Renderable2D(position, color, nullptr) { }
Renderable2D::Renderable2D(const vec2& position, Texture2D* texture) : Renderable2D(position, 0xFFFFFFFF, texture) { }

