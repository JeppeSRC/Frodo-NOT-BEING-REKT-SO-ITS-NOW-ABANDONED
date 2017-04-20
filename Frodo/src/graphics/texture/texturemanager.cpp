#include "texturemanager.h"
#include "texture2d.h"
#include <core/log.h>
#include <math/vec4.h>

namespace FD {

Map<char*, Texture*> TextureManager::textures;

void TextureManager::Init() {
	textures.Reserve(256);

	vec4 a[4]{
		vec4(1, 1, 1, 1),
		vec4(1, 1, 1, 1),
		vec4(1, 1, 1, 1),
		vec4(1, 1, 1, 1)
	};

	String s("default");

	Add(s, new Texture2D(a, 2, 2, FD_TEXTURE_FORMAT_FLOAT_32_32_32_32));
}

void TextureManager::Dispose() {
	textures.GetDataList().Free();
}

void TextureManager::Add(const String& name, Texture* tex) {
	for (uint_t i = 0; i < textures.GetItems(); i++) {
		String curr(textures.GetKeyList()[i]);
		if (name == curr) {
			FD_FATAL("\"%s\" already exist!", *name);
			return;
		}
	}

	char* tmp = new char[name.length + 1];
	memcpy(tmp, name.str, name.length + 1);

	textures.Add(tmp, tex);
}

Texture* TextureManager::Get(const String& name) {

	char* key = nullptr;

	for (uint_t i = 0; i < textures.GetItems(); i++) {
		key = textures.GetKeyList()[i];
		if (name == String(key)) {
			return textures.Retrieve(key);
		}
	}

	FD_FATAL("Couldn't find texture \"%s\"", *name);

	return nullptr;
}

}