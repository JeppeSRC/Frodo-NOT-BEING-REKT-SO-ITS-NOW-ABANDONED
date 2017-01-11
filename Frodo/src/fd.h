#pragma once

#include "core/fdtypes.h"

#ifdef FD_BUILD
#define FDAPI __declspec(dllexport)
#else
#define FDAPI __declspec(dllimport)
#endif

#ifdef NV_OPTIMUS_ENABLE
extern "C" __declspec(dllexport) uint32 NvOptimusEnablement = 0x1;
#undef NV_OPTIMUS_ENABLE
#endif
