#include "font.h"
#include <core/log.h>
#include <util/fileutils.h>
#include <util/vfs/vfs.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>

namespace FD {

Font* Font::defaultFont = nullptr;

Texture2D* GetCharFromFont(const String& fontName, uint32 character) {

	FT_Library library;
	FT_Face face;

	FT_Init_FreeType(&library);

	FT_New_Face(library, *fontName, 0, &face);

	FT_Select_Charmap(face, FT_ENCODING_UNICODE);

	if (face->face_flags & FT_FACE_FLAG_SCALABLE) {
		//FT_Set_Pixel_Sizes(face, 32, 32);
		FT_Set_Char_Size(face, 32 * 64, 0, 96, 96);
	}

	uint32 glyph_index = FT_Get_Char_Index(face, character);

	FT_GlyphSlot glyph = face->glyph;

	FT_Load_Glyph(face, glyph_index, 0);

	if (glyph->format != FT_GLYPH_FORMAT_BITMAP) {
		FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
	}


	uint32 advance = (glyph->advance.x >> 6);


	Texture2D* tex = new Texture2D(glyph->bitmap.buffer, glyph->bitmap.width, glyph->bitmap.rows, FD_TEXTURE_FORMAT_UINT_8);

	FT_Load_Glyph(face, glyph_index + 1, 0);

	return tex;
}

Font::Font(const String& fontFile, uint32 size, ivec2 dpi, FD_RANGE<>* range, uint32 num_ranges) {
	uint_t memory_size = 0;
	data = VFS::Get()->ReadFile(fontFile, &memory_size);

	this->num_ranges = num_ranges;

	ranges = new FD_RANGE<>[num_ranges];
	memcpy(ranges, range, num_ranges * sizeof(FD_RANGE<>));


	if (!(initialized = LoadFontFileInternal(data, (uint32)memory_size, size, dpi, range, num_ranges))) {
		FD_WARNING("Failed to open font: \"%s\"", *fontFile);
	}
}

Font::Font(void* memory, uint32 memory_size, uint32 size, ivec2 dpi, FD_RANGE<>* range, uint32 num_ranges) {
	this->num_ranges = num_ranges;

	ranges = new FD_RANGE<>[num_ranges];
	memcpy(ranges, range, num_ranges * sizeof(FD_RANGE<>));
	
	data = new byte[memory_size];
	memcpy(data, memory, memory_size);

	if (!(initialized = LoadFontFileInternal(data, memory_size, size, dpi, range, num_ranges))) {
		FD_WARNING("Failed to open font: \"%s\"", "nullptr(\"From memory\")");
	}
}

Font::~Font() {
	delete[] data;
	delete texture;
	delete[] ranges;
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

bool Font::LoadFontFileInternal(byte* memory, uint32 memory_size, uint32 size, ivec2 dpi, FD_RANGE<>* ranges, uint32 num_ranges) {
	this->size = size;
	uint32 numCharacters = 0;

	for (uint_t i = 0; i < num_ranges; i++)
		numCharacters += ranges[i].GetDistance();

	FT_Init_FreeType(&library);

	if (FT_New_Memory_Face(library, memory, memory_size, 0, &face)) {
		FD_DEBUG("Failed to open new face");
		return false;
	}

	this->name = String(face->family_name);

	FT_Set_Char_Size(face, size * 64, 0, dpi.x, dpi.y);
	FT_Select_Charmap(face, FT_ENCODING_UNICODE);

	uint32 segmentWidth = 0;
	uint32 segmentHeight = 0;

	for (uint_t i = 0; i < num_ranges; i++) {
		FD_RANGE<> range = ranges[i];
		for (int32 c = range.start; c < range.end; c++) {

			uint32 index = FT_Get_Char_Index(face, c);
	
			if (index == 0) continue;

			FT_Load_Glyph(face, index, 0);

			FT_GlyphSlot glyphSlot = face->glyph;
			FT_Render_Glyph(glyphSlot, FT_RENDER_MODE_NORMAL);

			FT_Bitmap bitmap = glyphSlot->bitmap;
			FT_Glyph_Metrics metrics = glyphSlot->metrics;

			FD_GLYPH glyph;

			glyph.unicodeCharacter = c;
			glyph.advance.x = glyphSlot->advance.x >> 6;
			glyph.offset.x = metrics.horiBearingX >> 6;
			glyph.offset.y = (metrics.horiBearingY  - metrics.height) >> 6;
			glyph.bitmapSize.x = bitmap.width;
			glyph.bitmapSize.y = bitmap.rows;

			uint32 bitmap_size = bitmap.rows * bitmap.width;

			glyph.bitmap = new byte[bitmap_size];

			memcpy(glyph.bitmap, bitmap.buffer, bitmap_size);

			if (glyph.bitmapSize.x > (int32)segmentWidth)  segmentWidth = glyph.bitmapSize.x;
			if (glyph.bitmapSize.y > (int32)segmentHeight) segmentHeight = glyph.bitmapSize.y;

			charMap.Add(c, glyph);
		}
	}

	uint32 bitmapSquareSize = (uint32)ceilf(sqrtf((float32)numCharacters));
	uint32 bitmapWidth = bitmapSquareSize * segmentWidth;
	uint32 bitmapHeight = bitmapSquareSize * segmentHeight;

	byte* bitmapData = new byte[bitmapWidth * bitmapHeight];
	memset(bitmapData, 0, bitmapWidth * bitmapHeight);

	float32 xStep = (float32)segmentWidth / (float32)bitmapWidth;
	float32 yStep = (float32)segmentHeight / (float32)bitmapHeight;
	float32 xOnePixelOffset = 1.0f / (float32)bitmapWidth;
	float32 yOnePixelOffset = 1.0f / (float32)bitmapHeight;

	uint32 currentGlyph = 0;

	for (uint_t i = 0; i < num_ranges; i++) {
		FD_RANGE<> range = ranges[i];
		for (int32 c = range.start; c < range.end; c++) {
			FD_GLYPH& glyph = charMap[c];

			int32 xStart = currentGlyph % bitmapSquareSize;
			int32 yStart = currentGlyph / bitmapSquareSize;

			glyph.u0 = (float32)xStart * xStep + xOnePixelOffset;
			glyph.v0 = (float32)yStart * yStep + yOnePixelOffset;
			glyph.u1 = glyph.u0 + xStep - xOnePixelOffset;
			glyph.v1 = glyph.v0 + yStep - yOnePixelOffset;

			uint32 yOffset = segmentHeight - glyph.bitmapSize.y;

			for (int32 y = 0; y < glyph.bitmapSize.y; y++) {
				int32 ya = (yStart * segmentHeight) + y + yOffset;
				for (int32 x = 0; x < glyph.bitmapSize.x; x++) {
					int32 xa = (xStart * segmentWidth) + x;
					bitmapData[xa + ya * bitmapWidth] = glyph.bitmap[x + y * glyph.bitmapSize.x];
				}
			}

			currentGlyph++;
			delete[] glyph.bitmap;
		}
	}

	texture = new Texture2D(bitmapData, bitmapWidth, bitmapHeight, FD_TEXTURE_FORMAT_UINT_8);

	delete[] bitmapData;

	return true;
}

ivec2 Font::GetKerning(uint32 left, uint32 right) {
	if (!left || !right) return ivec2(0, 0);
	FT_Vector kerning;

	uint32 lI = FT_Get_Char_Index(face, left);
	uint32 rI = FT_Get_Char_Index(face, right);
	uint32 kern = FT_HAS_KERNING(face);
	FT_Get_Kerning(face, lI, rI, FT_KERNING_DEFAULT, &kerning);

	return ivec2(kerning.x, kerning.y);
}


vec2 Font::GetFontMetrics(const String& string, vec2 scale) const {
	uint_t len = string.length;

	vec2 total(0, ((float32)size * scale.y));

	float maxLength = 0;

	for (uint_t i = 0; i < len; i++) {
		uint32 c = string[i];
		if (c == ' ') {
			total.x += (((float)size * scale.x) * 0.5f);
			continue;
		}
		else if (c == '\n') {
			total.y += ((float)size * scale.y);
			maxLength = MAX(maxLength, total.x);
			total.x = 0;
		}


		const FD_GLYPH& glyph = charMap.Retrieve(c);
		
		total.x += ((float32)glyph.advance.x * scale.x);

	}

	total.x = MAX(maxLength, total.x);

	return total;
}

vec2 Font::GetFontMetrics(const char character, vec2 scale) const {
	const FD_GLYPH& glyph = charMap.Retrieve(character);

	return vec2((float32)glyph.advance.x * scale.x, (float32)size * scale.y);
}

vec2 Font::GetScaleFromSize(uint32 size) const {
	float32 scale = (float32)size / this->size;
	return vec2(scale, scale);
}

bool Font::IsCharLoaded(uint32 c) const {
	for (uint_t i = 0; i < num_ranges; i++) {
		FD_RANGE<> r = ranges[i];
		if ((int32)c >= r.start && (int32)c <= r.end) return true;
	}

	return false;
}

}