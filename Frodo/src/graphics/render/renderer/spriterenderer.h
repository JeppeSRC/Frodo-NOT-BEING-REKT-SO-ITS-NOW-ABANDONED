#pragma once

#include "batchrenderer.h"

#include <entity/entity.h>

namespace FD {

class FDAPI SpriteRenderer : public BatchRenderer {
protected:


public:
	SpriteRenderer(Window* window, uint32 max_sprites, uint32 max_simultaneous_textures);
	~SpriteRenderer();

	virtual void Submit(const Sprite* sprite);

};


}