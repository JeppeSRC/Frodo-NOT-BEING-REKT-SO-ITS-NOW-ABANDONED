#pragma once

#include <util/string.h>
#include <util/list.h>
#include <util/fileutils.h>

#include <math/math.h>

#include <core/log.h>



bool ConvertOBJToFDM(const FD::String& filename, const FD::String& newFilename, uint32 attributes);
