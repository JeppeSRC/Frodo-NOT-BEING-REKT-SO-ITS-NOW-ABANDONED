#pragma once

#include "texture.h"
#include <util/map.h>

class FDAPI TextureManager {
private:
	static Map<String, Texture*> textures;

public:
	static void Init();
	static void Dispose();

	static void Add(const String& name, Texture* tex);
	static Texture* Get(const String& name);
};
