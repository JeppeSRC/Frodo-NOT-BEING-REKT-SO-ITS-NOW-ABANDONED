#pragma once

#include "string.h"
#include <stdio.h>

namespace FD {


String FDReadTextFile(const String& filename);
byte* FDReadBinaryFile(const String& filename, uint_t* fileSize);

uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size);
uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size, uint64 offset);
uint_t FDWriteFile(FILE* file, const void* buffer, uint64 size, uint64* offset);

}