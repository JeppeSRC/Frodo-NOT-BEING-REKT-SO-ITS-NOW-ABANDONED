#pragma once


#ifdef FD_BUILD
#define FDAPI __declspec(dllexport)
#else
#define FDAPI __declspec(dllimport)
#endif


#define NV_OPTIMUS extern "C" __declspec(dllexport) unsigned int NvOptimusEnablement = 0x1;
#define AMD_POWERXPRESS extern "C" __declspec(dllexport) unsigned int AmdPowerXpressRequestHighPerformance = 0x1;

#ifdef NV_OPTIMUS_ENABLE
NV_OPTIMUS
#undef NV_OPTIMUS_ENABLE
#endif

#ifdef AMD_POWERXPRESS_ENABLE
AMD_POWERXPRESS
#undef AMD_POWERXPRESS_ENABLE
#endif

#ifdef GPU_REQUEST_HIGH_PERFORMANCE
NV_OPTIMUS
AMD_POWERXPRESS
#undef GPU_REQUEST_HIGH_PERFORMANCE
#endif