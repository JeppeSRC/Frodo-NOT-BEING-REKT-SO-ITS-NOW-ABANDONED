#include "texture.h"
#include <core/log.h>
#include <util/vfs/vfs.h>
#include <FreeImage.h>

unsigned char* FDLoadImage(const String& filename, unsigned int* width, unsigned int* height, unsigned int* bits) {
	FD_ASSERT((width != nullptr && "width parameter nullptr"));
	FD_ASSERT((height != nullptr && "height parameter nullptr"));
	FD_ASSERT((bits != nullptr && "bits parameter nullptr"));
	
	size_t size = 0;
	
	unsigned char* rawData = VFS::Get()->ReadFile(filename, &size);

	FIMEMORY* data = FreeImage_OpenMemory(rawData, size);

	FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(data, size);

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

	bitmap = FreeImage_LoadFromMemory(format, data);

	*width = FreeImage_GetWidth(bitmap);
	*height = FreeImage_GetHeight(bitmap);
	*bits = FreeImage_GetBPP(bitmap);
	
	unsigned char* pixels = new unsigned char[*width * *height * (*bits / 8)];
	memcpy(pixels, FreeImage_GetBits(bitmap), *width * *height * (*bits / 8));

	FreeImage_Unload(bitmap);
	FreeImage_CloseMemory(data);


	return pixels;
}