#pragma once

#include <fd.h>
#include <util/string.h>
#include <util/map.h>
#include <math/vec2.h>
#include <graphics/texture/texture2d.h>

FDAPI Texture2D* GetCharFromFont(const String& fontName, unsigned int character);

class FDAPI Font {
private:
	friend struct FT_LibraryRec_;
	friend struct FT_FaceRec_;
public:
	
	template<typename T = int>
	struct FD_RANGE {
		T start;
		T end;

		inline T GetDistance() const { 
			return end - start;
		}
	};

	struct FD_GLYPH {
		unsigned int unicodeCharacter = 0;

		ivec2 offset;

		ivec2 advance;

		float u0;
		float u1;
		float v0;
		float v1;

		ivec2 bitmapSize;

		byte* bitmap;

		inline bool operator==(unsigned int unicodeCharacter) const { return this->unicodeCharacter == unicodeCharacter; }
	};

private:
	String name;
	unsigned int size;

	Texture2D* texture;
	Map<unsigned int, FD_GLYPH> charMap;

	FT_LibraryRec_* library;
	FT_FaceRec_* face;

	bool initialized;

	bool LoadFontFileInternal(unsigned char* memory, unsigned int memory_size, unsigned int size, ivec2 dpi, FD_RANGE<>* ranges, unsigned int num_ranges);

public:
	Font(const String& fontFile, unsigned int size, ivec2 dpi);
	Font(void* memory, unsigned int memory_size, unsigned int size, ivec2 dpi);
	~Font();

	ivec2 GetKerning(unsigned int left, unsigned int right);
	ivec2 GetFontMetrics(const String& string) const;

	inline const String& GetName() const { return name; }
	inline unsigned int GetSize() const { return size; }
	inline Texture2D* GetTexture() const { return texture; }
	inline FD_GLYPH GetGlyph(unsigned int unicodeCharacter) { return charMap.Retrieve(unicodeCharacter); }
};

