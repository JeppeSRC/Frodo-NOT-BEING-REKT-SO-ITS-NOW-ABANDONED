#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4099)

#ifdef FD_BUILD
#define FDAPI __declspec(dllexport)
#else
#define FDAPI __declspec(dllimport)
#endif
