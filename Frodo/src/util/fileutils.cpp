#include "fileutils.h"
#include <stdio.h>
#include <core/log.h>

#define FREAD(buff, size, file) fread(buff, size, 1, file)
#define FWRITE(buff, size, file) fwrite(buff, size, 1, file)


#ifdef _M_X64

#define FTELL(file) _ftelli64(file)
#define FSEEK(file, off, org) _fseeki64(file, off, org)

#else

#define FTELL(file) ftell(file)
#define FSEEK(file, off, org) fseek(file, off, org)

#endif

#define FSIZE(dst, file) FSEEK(file, 0, SEEK_END); \
dst = FTELL(file); \
FSEEK(file, 0, SEEK_SET);

String FDReadTextFile(const String& filename) {

	FILE* file = fopen(*filename, "rb");

	if (!file) {
		FD_FATAL("Failed to open file \"%s\"", *filename);
		return String("Failed to open file");
	}

	FSIZE(size_t length, file);

	char* buff = new char[length+1];

	FREAD(buff, length, file);

	buff[length] = '\0';
	fclose(file);

	return String(buff);
}

unsigned char* FDReadBinaryFile(const String& filename, size_t* fileSize) {

	FILE* file = fopen(*filename, "rb");

	if (!file) {
		FD_FATAL("Failed to open file \"%s\"", *filename);
		return nullptr;
	}

	FSIZE(*fileSize, file);

	unsigned char* buff = new unsigned char[*fileSize];

	FREAD(buff, *fileSize, file);

	fclose(file);

	return buff;
}