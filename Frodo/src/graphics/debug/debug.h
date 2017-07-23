#pragma once

#include <fd.h>

#include <graphics/texture/texture.h>

namespace FD {

class FDAPI Debug {
public:
	static void ShowTexture(Texture* tex);
};

}
