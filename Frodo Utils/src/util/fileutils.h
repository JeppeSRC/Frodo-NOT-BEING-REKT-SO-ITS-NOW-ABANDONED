#pragma once

#include "string.h"
#include <stdio.h>

namespace FD {


FDAPI String FDReadTextFile(const String& filename);
FDAPI byte*  FDReadBinaryFile(const String& filename, uint_t* fileSize);

FDAPI uint_t FDWriteFile(const String& filename, const void* buffer, uint64 size);
FDAPI uint_t FDWriteFile(const String& filename, const void* buffer, uint64 size, uint64 offset);
FDAPI uint_t FDWriteFile(const String& filename, const void* buffer, uint64 size, uint64* offset);
	  
FDAPI uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size);
FDAPI uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size, uint64 offset);
FDAPI uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size, uint64* offset);

}