#pragma once

#include <fd.h>
#include <util/string.h>
#include <util/map.h>
#include <math/vec2.h>
#include <graphics/texture/texture2d.h>

FDAPI Texture2D* GetCharFromFont(const String& fontName, unsigned int character);

class FDAPI Font {
public:
	
	template<typename T = int>
	struct FD_RANGE {
		T start;
		T end;

		inline T GetDistance() const { 
			return end - start;
		}
	};

private:

	struct FD_GLYPH {
		unsigned int unicodeCharacter;

		ivec2 offset;

		ivec2 advance;

		vec2 texCoords;

		ivec2 bitmapSize;

		byte* bitmap;

		inline bool operator==(unsigned int unicodeCharacter) const { return this->unicodeCharacter == unicodeCharacter; }
	};

private:
	String name;
	unsigned int size;

	Texture2D* texture;
	Map<unsigned int, FD_GLYPH> charMap;

	bool initialized;

	bool LoadFontFileInternal(unsigned char* memory, unsigned int memory_size, unsigned int size, ivec2 dpi, FD_RANGE<>* ranges, unsigned int num_ranges);

public:
	Font(const String& fontFile, unsigned int size, ivec2 dpi);
	Font(void* memory, unsigned int memory_size, unsigned int size, ivec2 dpi);
	~Font();
};

