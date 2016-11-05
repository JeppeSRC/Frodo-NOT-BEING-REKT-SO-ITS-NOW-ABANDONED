#include "font.h"
#include <core/log.h>
#include <util/fileutils.h>
#include <util/vfs/vfs.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>


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

	if (!(initialized = LoadFontFileInternal(data, memory_size, size, dpi, &r, 1))) {
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

}

bool Font::LoadFontFileInternal(unsigned char* memory, unsigned int memory_size, unsigned int size, ivec2 dpi, FD_RANGE<>* ranges, unsigned int num_ranges) {
	this->size = size;

	FT_Library library;
	FT_Face face;

	unsigned int num_characters = 0;

	for (size_t i = 0; i < num_ranges; i++)
		num_characters += ranges[i].GetDistance();

	FT_Init_FreeType(&library);

	if (FT_New_Memory_Face(library, memory, memory_size, 0, &face)) {
		FD_DEBUG("Failed to open new face");
		return false;
	}

	this->name = String(face->family_name);

	FT_Set_Char_Size(face, size * 64, 0, dpi.x, dpi.y);
	FT_Select_Charmap(face, FT_ENCODING_UNICODE);

	unsigned int segment_width = 0;
	unsigned int segment_height = 0;

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
			glyph.offset.y = (metrics.horiBearingY - metrics.height) >> 6;
			glyph.bitmapSize.x = bitmap.width;
			glyph.bitmapSize.y = bitmap.rows;

			unsigned int bitmap_size = bitmap.rows * bitmap.width;

			glyph.bitmap = new unsigned char[bitmap_size];

			memcpy(glyph.bitmap, bitmap.buffer, bitmap_size);

			if (glyph.bitmapSize.x > segment_width) segment_width = glyph.bitmapSize.x;
			if (glyph.bitmapSize.y > segment_height) segment_height = glyph.bitmapSize.y;

			charMap.Add(glyph, c);
		}
	}
	
	unsigned int bitmapSquareSize = (unsigned int)ceilf(sqrtf((float)num_characters));
	unsigned int bitmapWidth = bitmapSquareSize * segment_width;
	unsigned int bitmapHeight = bitmapSquareSize * segment_height;

	unsigned char* bitmapData = new unsigned char[bitmapWidth * bitmapHeight];
	memset(bitmapData, 0, bitmapWidth * bitmapHeight);

	float xStep = (float)segment_width / (float)bitmapWidth;
	float yStep = (float)segment_height / (float)bitmapHeight;

	unsigned int currentGlyph = 0;

	for (size_t i = 0; i < num_ranges; i++) {
		FD_RANGE<> range = ranges[i];
		for (int c = range.start; c < range.end; c++) {
			FD_GLYPH& glyph = charMap[c];

			int xStart = currentGlyph % bitmapSquareSize;
			int yStart = currentGlyph / bitmapSquareSize;

			glyph.texCoords.x = (float)xStart * xStep;
			glyph.texCoords.y = (float)yStart * yStep;

			for (int y = 0; y < glyph.bitmapSize.y; y++) {
				int ya = (yStart * segment_height) + y;
				for (int x = 0; x < glyph.bitmapSize.x; x++) {
					int xa = (xStart * segment_width) + x;
					bitmapData[xa + ya * bitmapWidth] = glyph.bitmap[x + y * glyph.bitmapSize.x];
				}
			}
			
			currentGlyph++;
			delete[] glyph.bitmap;
		}
	}

	texture = new Texture2D(bitmapData, bitmapWidth, bitmapHeight, FD_TEXTURE_FORMAT_UINT_8);
	
	return true;
}