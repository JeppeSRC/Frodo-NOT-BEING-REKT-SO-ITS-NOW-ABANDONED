#pragma once

#include "string.h"
#include <stdio.h>

String FDReadTextFile(const String& filename);
byte* FDReadBinaryFile(const String& filename, uint_t* fileSize);

bool FDWriteFile(FILE* file, const void* buffer, uint64 size);
bool FDWriteFile(FILE* file, const void* buffer, uint64 size, uint64 offset);
bool FDWriteFile(FILE* file, const void* buffer, uint64 size, uint64* offset);