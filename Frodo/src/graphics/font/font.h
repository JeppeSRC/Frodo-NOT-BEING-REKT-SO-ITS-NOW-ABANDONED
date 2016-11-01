#pragma once

#include <fd.h>
#include <util/string.h>
#include <util/map.h>
#include <graphics/texture/texture2d.h>

class FDAPI Font {
public:
	static Texture2D* GetCharFromFont(const String& fontName, unsigned int character);

};

