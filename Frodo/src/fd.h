#pragma once


#ifdef FD_BUILD
#define FDAPI __declspec(dllexport)
#else
#define FDAPI __declspec(dllimport)
#endif
