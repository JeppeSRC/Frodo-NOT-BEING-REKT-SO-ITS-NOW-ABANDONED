#include "texture.h"
#include <core/log.h>
#include <util/vfs/vfs.h>
#include <FreeImage.h>


namespace FD {


byte* Texture::Load(const String& filename, uint32* width, uint32* height, uint32* bits, bool flipY) {
	FD_ASSERT((width != nullptr && "width parameter nullptr"));
	FD_ASSERT((height != nullptr && "height parameter nullptr"));
	FD_ASSERT((bits != nullptr && "bits parameter nullptr"));

	uint_t size = 0;

	byte* rawData = VFS::Get()->ReadFile(filename, &size);

	FIMEMORY* data = FreeImage_OpenMemory(rawData, size);

	FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(data, size);

	FIBITMAP* dib = nullptr;
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
	
	dib = FreeImage_LoadFromMemory(format, data);

	bitmap = FreeImage_ConvertTo32Bits(dib);
	

	FreeImage_Unload(dib);

	*width = FreeImage_GetWidth(bitmap);
	*height = FreeImage_GetHeight(bitmap);

	*bits = 32;// FreeImage_GetBPP(bitmap);
	
	uint_t imageSize = *width * *height * (*bits / 8);
	byte* pixels = new byte[imageSize];

	if (flipY) {
		FreeImage_FlipVertical(bitmap);
	}
	
	memcpy(pixels, FreeImage_GetBits(bitmap), imageSize);

	if (FreeImage_GetRedMask(bitmap) == 0xFF0000) {
		//Swap red and blue
		for (uint_t i = 0; i < imageSize; i += (*bits / 8)) {
			byte* pixel = pixels + i;
			byte tmp = pixel[0];
			pixel[0] = pixel[2];
			pixel[2] = tmp;
		}
	}


	FreeImage_Unload(bitmap);
	FreeImage_CloseMemory(data);


	return pixels;
}

byte* Texture::Load(void* memory, uint32* width, uint32* height, uint32* bits, bool flipY) {
	FD_ASSERT((width != nullptr && "width parameter nullptr"));
	FD_ASSERT((height != nullptr && "height parameter nullptr"));
	FD_ASSERT((bits != nullptr && "bits parameter nullptr"));

	uint_t size = 0;

	FIMEMORY* data = FreeImage_OpenMemory((byte*)memory, size);

	FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(data, size);

	FIBITMAP* dib = nullptr;
	FIBITMAP* bitmap = nullptr;

	if (format == FIF_UNKNOWN) {
		FD_FATAL("[Texture] unknown file type");
		*width = 0;
		*height = 0;
		*bits = 0;
		return nullptr;
	}


	if (!FreeImage_FIFSupportsReading(format)) {
		FD_WARNING("[Texture]  does not support reading");
	}

	dib = FreeImage_LoadFromMemory(format, data);

	bitmap = FreeImage_ConvertTo32Bits(dib);


	FreeImage_Unload(dib);

	*width = FreeImage_GetWidth(bitmap);
	*height = FreeImage_GetHeight(bitmap);

	*bits = 32;// FreeImage_GetBPP(bitmap);

	uint_t imageSize = *width * *height * (*bits / 8);
	byte* pixels = new byte[imageSize];

	if (flipY) {
		FreeImage_FlipVertical(bitmap);
	}

	memcpy(pixels, FreeImage_GetBits(bitmap), imageSize);

	if (FreeImage_GetRedMask(bitmap) == 0xFF0000) {
		//Swap red and blue
		for (uint_t i = 0; i < imageSize; i += (*bits / 8)) {
			byte* pixel = pixels + i;
			byte tmp = pixel[0];
			pixel[0] = pixel[2];
			pixel[2] = tmp;
		}
	}


	FreeImage_Unload(bitmap);
	FreeImage_CloseMemory(data);

	return pixels;
}

}