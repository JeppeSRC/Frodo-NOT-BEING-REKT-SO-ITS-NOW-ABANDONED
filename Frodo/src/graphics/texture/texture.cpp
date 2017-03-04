#include "texture.h"
#include <core/log.h>
#include <util/vfs/vfs.h>
#include <FreeImage.h>

byte* Texture::Load(const String& filename, uint32* width, uint32* height, uint32* bits) {
	FD_ASSERT((width != nullptr && "width parameter nullptr"));
	FD_ASSERT((height != nullptr && "height parameter nullptr"));
	FD_ASSERT((bits != nullptr && "bits parameter nullptr"));
	
	uint_t size = 0;
	
	FIMEMORY data;
	data.data = VFS::Get()->ReadFile(filename, &size);

	FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(&data);

	FIBITMAP* bitmap = nullptr;

	if (format == FIF_UNKNOWN) {
		format = FreeImage_GetFIFFromFilename(*filename);
		if (format == FIF_UNKNOWN) {
			FD_FATAL("[Texture] \"%s\" unknown file type", *filename);
			*width = 0;
			*height = 0;
			*bits = 0;
			return nullptr;
		}
	}

	if (!FreeImage_FIFSupportsReading(format)) {
		FD_WARNING("[Texture] \"%s\" does not support reading", *filename);
	}

	bitmap = FreeImage_LoadFromMemory(format, &data);

	*width = FreeImage_GetWidth(bitmap);
	*height = FreeImage_GetHeight(bitmap);
	*bits = FreeImage_GetBPP(bitmap);
	
	byte* pixels = FreeImage_GetBits(bitmap);

	FreeImage_Unload(bitmap);
	delete data.data;

	return pixels;
}

byte* Texture::Load(void* memory, uint32* width, uint32* height, uint32* bits) {
	FD_ASSERT((width != nullptr && "width parameter nullptr"));
	FD_ASSERT((height != nullptr && "height parameter nullptr"));
	FD_ASSERT((bits != nullptr && "bits parameter nullptr"));

	uint_t size = 0;

	FIMEMORY data;
	data.data = memory;

	FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(&data);

	FIBITMAP* bitmap = nullptr;

	if (format == FIF_UNKNOWN) {
		FD_FATAL("[Texture] unknown file type");
		*width = 0;
		*height = 0;
		*bits = 0;
		return nullptr;
	}

	if (!FreeImage_FIFSupportsReading(format)) {
		FD_WARNING("[Texture] texture does not support reading");
	}

	bitmap = FreeImage_LoadFromMemory(format, &data);

	*width = FreeImage_GetWidth(bitmap);
	*height = FreeImage_GetHeight(bitmap);
	*bits = FreeImage_GetBPP(bitmap);

	byte* pixels = FreeImage_GetBits(bitmap);

	FreeImage_Unload(bitmap);

	return pixels;
}