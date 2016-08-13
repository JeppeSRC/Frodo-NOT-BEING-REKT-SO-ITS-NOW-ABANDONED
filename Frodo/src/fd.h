#pragma once


#ifdef FD_BUILD
#define FDAPI __declspec(dllexport)
#else
#define FDAPI __declspec(dllimport)
#endif

#ifdef NV_OPTIMUS_ENABLE
extern "C" __declspec(dllexport) unsigned int NvOptimusEnablement = 0x1;
#undef NV_OPTIMUS_ENABLE
#endif
