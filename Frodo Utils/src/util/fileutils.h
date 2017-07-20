#pragma once

#include "string.h"
#include <stdio.h>

namespace FD {


FDUAPI String FDReadTextFile(const String& filename);
FDUAPI byte*  FDReadBinaryFile(const String& filename, uint_t* fileSize);

FDUAPI uint_t FDWriteFile(const String& filename, const void* buffer, uint64 size);
FDUAPI uint_t FDWriteFile(const String& filename, const void* buffer, uint64 size, uint64 offset);
FDUAPI uint_t FDWriteFile(const String& filename, const void* buffer, uint64 size, uint64* offset);
	  
FDUAPI uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size);
FDUAPI uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size, uint64 offset);
FDUAPI uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size, uint64* offset);

}