#include "font.h"
#include <core/log.h>

#include <ft2build.h>
#include FT_FREETYPE_H


Texture2D* Font::GetCharFromFont(const String& fontName, unsigned int character) {

	FT_Library library;
	FT_Face face;

	FT_Init_FreeType(&library);

	FT_New_Face(library, *fontName, 0, &face);

	FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	
	if (face->face_flags & FT_FACE_FLAG_SCALABLE) {
		//FT_Set_Pixel_Sizes(face, 64, 64);
		FT_Set_Char_Size(face, 64, 0, 1000, 600);
	}

	unsigned int glyph_index = FT_Get_Char_Index(face, character);

	FT_GlyphSlot glyph = face->glyph;

	FT_Load_Glyph(face, glyph_index, 0);

	if (glyph->format != FT_GLYPH_FORMAT_BITMAP) {
		FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
	}


	unsigned int advance = (glyph->advance.x >> 6);
	

	Texture2D* tex = new Texture2D(glyph->bitmap.buffer, glyph->bitmap.width, glyph->bitmap.rows, FD_TEXTURE_FORMAT_UINT_8);


	return tex;
}