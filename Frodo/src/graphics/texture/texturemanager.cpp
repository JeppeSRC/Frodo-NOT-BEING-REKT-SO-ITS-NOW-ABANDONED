#include "texturemanager.h"
#include "texture2d.h"
#include <core/log.h>

Map<String, Texture*> TextureManager::textures;

void TextureManager::Init() {
	textures.Reserve(256);
	unsigned int a[4]{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
	Add("default", new Texture2D(a, 2, 2, FD_TEXTURE_FORMAT_UINT_8_8_8_8));
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