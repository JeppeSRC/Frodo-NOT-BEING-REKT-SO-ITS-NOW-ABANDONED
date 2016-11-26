#include "font.h"
#include <core/log.h>
#include <util/fileutils.h>
#include <util/vfs/vfs.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>

Font* Font::defaultFont = nullptr;


Texture2D* GetCharFromFont(const String& fontName, unsigned int character) {

	FT_Library library;
	FT_Face face;

	FT_Init_FreeType(&library);

	FT_New_Face(library, *fontName, 0, &face);

	FT_Select_Charmap(face, FT_ENCODING_UNICODE);

	if (face->face_flags & FT_FACE_FLAG_SCALABLE) {
		//FT_Set_Pixel_Sizes(face, 32, 32);
		FT_Set_Char_Size(face, 32 * 64, 0, 96, 96);
	}

	unsigned int glyph_index = FT_Get_Char_Index(face, character);

	FT_GlyphSlot glyph = face->glyph;

	FT_Load_Glyph(face, glyph_index, 0);

	if (glyph->format != FT_GLYPH_FORMAT_BITMAP) {
		FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
	}


	unsigned int advance = (glyph->advance.x >> 6);


	Texture2D* tex = new Texture2D(glyph->bitmap.buffer, glyph->bitmap.width, glyph->bitmap.rows, FD_TEXTURE_FORMAT_UINT_8);

	FT_Load_Glyph(face, glyph_index+1, 0);

	return tex;
}


Font::Font(const String& fontFile, unsigned int size, ivec2 dpi) { 
	size_t memory_size = 0;
	unsigned char* data = VFS::Get()->ReadFile(fontFile, &memory_size);

	FD_RANGE<> r;

	r.start = 0x21;
	r.end = 0x7E;

	if (!(initialized = LoadFontFileInternal(data, (unsigned int)memory_size, size, dpi, &r, 1))) {
		FD_WARNING("Failed to open font: \"%s\"", *fontFile);
	}

	delete[] data;
}

Font::Font(void* memory, unsigned int memory_size, unsigned int size, ivec2 dpi) {
	FD_RANGE<> r;

	r.start = 0x21;
	r.end = 0x7E;

	if (!(initialized = LoadFontFileInternal((unsigned char*)memory, memory_size, size, dpi, &r, 1))) {
		FD_WARNING("Failed to open font: \"%s\"", "nullptr(\"From memory\")");
	}
}

Font::~Font() {
	delete texture;
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

bool Font::LoadFontFileInternal(unsigned char* memory, unsigned int memory_size, unsigned int size, ivec2 dpi, FD_RANGE<>* ranges, unsigned int num_ranges) {
	this->size = size;
	unsigned int numCharacters = 0;

	for (size_t i = 0; i < num_ranges; i++)
		numCharacters += ranges[i].GetDistance();

	FT_Init_FreeType(&library);

	if (FT_New_Memory_Face(library, memory, memory_size, 0, &face)) {
		FD_DEBUG("Failed to open new face");
		return false;
	}

	this->name = String(face->family_name);

	FT_Set_Char_Size(face, size * 64, 0, dpi.x, dpi.y);
	FT_Select_Charmap(face, FT_ENCODING_UNICODE);

	unsigned int segmentWidth = 0;
	unsigned int segmentHeight = 0;

	for (size_t i = 0; i < num_ranges; i++) {
		FD_RANGE<> range = ranges[i];
		for (int c = range.start; c < range.end; c++) {

			unsigned int index = FT_Get_Char_Index(face, c);

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
			glyph.offset.y = (metrics.horiBearingY >> 6) - (metrics.height >> 6);
			glyph.bitmapSize.x = bitmap.width;
			glyph.bitmapSize.y = bitmap.rows;

			unsigned int bitmap_size = bitmap.rows * bitmap.width;

			glyph.bitmap = new unsigned char[bitmap_size];

			memcpy(glyph.bitmap, bitmap.buffer, bitmap_size);

			if (glyph.bitmapSize.x > segmentWidth)  segmentWidth  = glyph.bitmapSize.x;
			if (glyph.bitmapSize.y > segmentHeight) segmentHeight = glyph.bitmapSize.y;
			
			charMap.Add(glyph, c);
		}
	}
	
	unsigned int bitmapSquareSize = (unsigned int)ceilf(sqrtf((float)numCharacters));
	unsigned int bitmapWidth = bitmapSquareSize * segmentWidth;
	unsigned int bitmapHeight = bitmapSquareSize * segmentHeight;

	unsigned char* bitmapData = new unsigned char[bitmapWidth * bitmapHeight];
	memset(bitmapData, 0, bitmapWidth * bitmapHeight);

	float xStep = (float)segmentWidth / (float)bitmapWidth;
	float yStep = (float)segmentHeight / (float)bitmapHeight;

	unsigned int currentGlyph = 0;

	for (size_t i = 0; i < num_ranges; i++) {
		FD_RANGE<> range = ranges[i];
		for (int c = range.start; c < range.end; c++) {
			FD_GLYPH& glyph = charMap[c];

			int xStart = currentGlyph % bitmapSquareSize;
			int yStart = currentGlyph / bitmapSquareSize;

			glyph.u0 = (float)xStart * xStep;
			glyph.v0 = (float)yStart * yStep;
			glyph.u1 = glyph.u0 + xStep;
			glyph.v1 = glyph.v0 + yStep;

			unsigned int yOffset = segmentHeight - glyph.bitmapSize.y;

			for (int y = 0; y < glyph.bitmapSize.y; y++) {
				int ya = (yStart * segmentHeight) + y + yOffset;
				for (int x = 0; x < glyph.bitmapSize.x; x++) {
					int xa = (xStart * segmentWidth) + x;
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

ivec2 Font::GetKerning(unsigned int left, unsigned int right) {
	if (!left || !right) return ivec2(0, 0);
	FT_Vector kerning;

	FT_Get_Kerning(face, FT_Get_Char_Index(face, left), FT_Get_Char_Index(face, right), FT_KERNING_DEFAULT, &kerning);

	return ivec2(kerning.x, kerning.y);
}

ivec2 Font::GetFontMetrics(const String& string, vec2 scale) const {
	size_t len = string.length;

	ivec2 total;

	for (size_t i = 0; i < len; i++) {
		unsigned int c = string[i];
		if (c == ' ') {
			total.x += size >> 1;
			continue;
		}

		const FD_GLYPH& glyph = charMap.Retrieve((unsigned int)string[i]);
		
		total.x += (unsigned int)(float(glyph.offset.x + (glyph.advance.x * (i < len-1 ? 1 : 0))) * scale.x);
	}

	total.y = size;

	return total;
}