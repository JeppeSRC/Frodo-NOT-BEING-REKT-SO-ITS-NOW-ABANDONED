#include "texturemanager.h"
#include "texture2d.h"
#include <core/log.h>
#include <math/vec4.h>

Map<String, Texture*> TextureManager::textures;

void TextureManager::Init() {
	textures.Reserve(256);
	
	vec4 a[4]{
		vec4(1, 0, 0, 1),
		vec4(0, 1, 0, 1),
		vec4(0, 0, 1, 1),
		vec4(1, 0, 1, 1)
	};

	Add("default", new Texture2D(a, 2, 2, FD_TEXTURE_FORMAT_FLOAT_32_32_32_32));
}

void TextureManager::Dispose() {
	textures.GetDataList().Free();
}

void TextureManager::Add(const String& name, Texture* tex) {

	if (textures.GetKeyList().Find(name) != (size_t)-1) {
		FD_FATAL("\"%s\" already exist!", *name);
		return;
	}

	textures.Add(tex, name);
}

Texture* TextureManager::Get(const String& name) {
	return textures.Retrieve(name);
}