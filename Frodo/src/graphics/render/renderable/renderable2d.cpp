#include "renderable2d.h"

Renderable2D::Renderable2D(const vec2& position, const vec2& scale, float rotation, unsigned int color, Texture2D* texture) {
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->color = color;
	this->texture = texture;

}

Renderable2D::Renderable2D(const vec2& position, const vec2& scale, unsigned int color, Texture2D* texture) : Renderable2D(position, scale, 0, color, texture) { }
Renderable2D::Renderable2D(const vec2& position, const vec2& scale, unsigned int color) : Renderable2D(position, scale, color, nullptr) { }
Renderable2D::Renderable2D(const vec2& position, const vec2& scale, Texture2D* texture) : Renderable2D(position, scale, 0xFFFFFFFF, texture) { }

Renderable2D::Renderable2D(const vec2& position, unsigned int color, Texture2D* texture) : Renderable2D(position, vec2(1, 1), color, texture) { }
Renderable2D::Renderable2D(const vec2& position, unsigned int color) : Renderable2D(position, color, nullptr) { }
Renderable2D::Renderable2D(const vec2& position, Texture2D* texture) : Renderable2D(position, 0xFFFFFFFF, texture) { }

