#pragma once

#include "string.h"

String FDReadTextFile(const String& filename);
unsigned char* FDReadBinaryFile(const String& filename, size_t* fileSize);
