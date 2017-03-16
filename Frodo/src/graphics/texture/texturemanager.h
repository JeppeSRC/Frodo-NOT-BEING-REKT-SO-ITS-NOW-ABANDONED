#pragma once

#include "texture.h"
#include <util/map.h>

namespace FD {

class FDAPI TextureManager {
private:
	static Map<char*, Texture*> textures;

public:
	static void Init();
	static void Dispose();

	static void Add(const String& name, Texture* tex);
	static Texture* Get(const String& name);
};

}