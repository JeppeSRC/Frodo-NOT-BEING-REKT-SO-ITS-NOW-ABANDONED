#pragma once

#include <fd.h>
#include <util/string.h>
#include <util/map.h>
#include <math/vec2.h>
#include <graphics/texture/texture2d.h>

struct FT_LibraryRec_;
struct FT_FaceRec_;

namespace FD {

FDAPI Texture2D* GetCharFromFont(const String& fontName, uint32 character);

class FDAPI Font {
private:
	friend struct FT_LibraryRec_;
	friend struct FT_FaceRec_;

	static Font* defaultFont;
public:

	template<typename T = int32>
	struct FD_RANGE {
		T start;
		T end;

		inline T GetDistance() const {
			return end - start;
		}
	};

	struct FD_GLYPH {
		uint32 unicodeCharacter = 0;

		ivec2 offset;

		ivec2 advance;

		float32 u0;
		float32 u1;
		float32 v0;
		float32 v1;

		ivec2 bitmapSize;

		byte* bitmap;

		inline bool operator==(uint32 unicodeCharacter) const { return this->unicodeCharacter == unicodeCharacter; }
	};

private:
	String name;
	uint32 size;

	Texture2D* texture;
	Map<uint32, FD_GLYPH> charMap;

	FT_LibraryRec_* library;
	FT_FaceRec_* face;

	bool initialized;

	bool LoadFontFileInternal(byte* memory, uint32 memory_size, uint32 size, ivec2 dpi, FD_RANGE<>* ranges, uint32 num_ranges);

public:
	Font(const String& fontFile, uint32 size, ivec2 dpi);
	Font(void* memory, uint32 memory_size, uint32 size, ivec2 dpi);
	~Font();

	ivec2 GetKerning(uint32 left, uint32 right);
	vec2 GetFontMetrics(const String& string, vec2 scale = vec2(1, 1)) const;

	inline const String& GetName() const { return name; }
	inline uint32 GetSize() const { return size; }
	inline Texture2D* GetTexture() const { return texture; }
	inline FD_GLYPH GetGlyph(uint32 unicodeCharacter) { return charMap.Retrieve(unicodeCharacter); }

	static inline Font* GetDefaultFont() { return defaultFont; }
	static inline void SetDefaultFont(Font* font) { Font::defaultFont = font; }
};

}