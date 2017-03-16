#include "fileutils.h"
#include <stdio.h>
#include <core/log.h>

#define FREAD(buff, size, file) fread(buff, size, 1, file)
#define FWRITE(buff, size, file) fwrite(buff, size, 1, file)


#define FTELL(file) _ftelli64(file)
#define FSEEK(file, off, org) _fseeki64(file, off, org)


#define FSIZE(dst, file) FSEEK(file, 0, SEEK_END); \
dst = FTELL(file); \
FSEEK(file, 0, SEEK_SET);

namespace FD {

String FDReadTextFile(const String& filename) {

	FILE* file = fopen(*filename, "rb");

	if (!file) {
		FD_FATAL("[FileUtils] Failed to open file \"%s\"", *filename);
		return String("Failed to open file");
	}

	FSIZE(uint_t length, file);

	char* buff = new char[length + 1];

	FREAD(buff, length, file);

	buff[length] = '\0';
	fclose(file);

	return String(buff);
}

byte* FDReadBinaryFile(const String& filename, uint_t* fileSize) {

	FILE* file = fopen(*filename, "rb");

	uint_t size;

	if (!file) {
		FD_FATAL("[FileUtils] Failed to open file \"%s\"", *filename);
		return nullptr;
	}

	FSIZE(size, file);

	byte* buff = new byte[size];

	FREAD(buff, size, file);

	fclose(file);

	if (fileSize != nullptr) *fileSize = size;

	return buff;
}

uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size) {
	return FWRITE(buffer, size, file);
}

uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size, uint64 offset) {
	FSEEK(file, offset, SEEK_SET);
	return FWRITE(buffer, size, file);
}

uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size, uint64* offset) {
	FSEEK(file, *offset, SEEK_SET);

	*offset += size;

	return FWRITE(buffer, size, file);
}

}