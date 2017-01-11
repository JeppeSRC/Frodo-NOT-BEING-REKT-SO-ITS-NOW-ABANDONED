#pragma once

#include "string.h"

String FDReadTextFile(const String& filename);
byte* FDReadBinaryFile(const String& filename, uint_t* fileSize);
