#pragma once

#ifdef FD_BUILD
#define FD_EXTERN_TEMPLATE
#define FDAPI __declspec(dllexport)
#else
#define FD_EXTERN_TEMPLATE extern
#define FDAPI __declspec(dllimport)
#endif

#ifndef FD_TYPES_DEFINED
typedef unsigned char		byte;
typedef unsigned short		uint16;
typedef signed   short		int16;
typedef unsigned int		uint32;
typedef signed   int		int32;
typedef unsigned long		uint32l;
typedef signed	 long		int32l;
typedef unsigned long long	uint64;
typedef signed   long long	int64;
typedef float				float32;
typedef double				float64;
typedef size_t				uint_t;
#define FD_TYPES_DEFINED
#endif