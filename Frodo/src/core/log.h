#pragma once

#include <fd.h>
#include <util/string.h>

namespace FD {

#define FD_LOG_LEVEL_INFO    0x01
#define FD_LOG_LEVEL_DEBUG   0x02
#define FD_LOG_LEVEL_WARNING 0x03
#define FD_LOG_LEVEL_FATAL   0x04

#define FD_INFO(msg, ...) FDLog(FD_LOG_LEVEL_INFO, msg, __VA_ARGS__)
#define FD_WARNING(msg, ...) FDLog(FD_LOG_LEVEL_WARNING, msg, __VA_ARGS__)
#define FD_FATAL(msg, ...) FDLog(FD_LOG_LEVEL_FATAL, msg, __VA_ARGS__)
#define FD_LOG_DISPLAY_DEBUG
#if defined(_DEBUG) || defined(FD_LOG_DISPLAY_DEBUG)
#define FD_DEBUG(msg, ...) FDLog(FD_LOG_LEVEL_DEBUG, msg, __VA_ARGS__)

#define FD_ASSERT_MSG(statement, msg) \
if (statement) { \
	FD_FATAL("ASSERTION FAILED: File: \"%s\" Func: \"%s\" Line: %u \"%s\"", __FILE__, __FUNCSIG__, __LINE__, msg); \
	int32* abcdefghijklmnopqrstuvwxyz123456789 = nullptr; \
	*abcdefghijklmnopqrstuvwxyz123456789 = 1; \
}

#define FD_ASSERT(statement) \
if (statement) { \
	FD_FATAL("ASSERTION FAILED: File: \"%s\" Func: \"%s\" Line: %u \"%s\"", __FILE__, __FUNCSIG__, __LINE__, #statement); \
	int32* abcdefghijklmnopqrstuvwxyz123456789 = nullptr; \
	*abcdefghijklmnopqrstuvwxyz123456789 = 1; \
}

#else
#define FD_DEBUG(msg, ...)
#define FD_ASSERT_MSG(statement, msg)
#define FD_ASSERT(statement)
#endif





void FDAPI FDLog(byte level, const char* message...);

}