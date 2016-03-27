#pragma once
#include <fd.h>
#include <util/string.h>


#define FD_LOG_LEVEL_INFO    0x01
#define FD_LOG_LEVEL_DEBUG   0x02
#define FD_LOG_LEVEL_WARNING 0x03
#define FD_LOG_LEVEL_FATAL   0x04

#define FD_INFO(msg, ...) FDLog(FD_LOG_LEVEL_INFO, msg, __VA_ARGS__)

#ifdef _DEBUG
#define FD_DEBUG(msg, ...) FDLog(FD_LOG_LEVEL_DEBUG, msg, __VA_ARGS__)
#define FD_WARNING(msg, ...) FDLog(FD_LOG_LEVEL_WARNING, msg, __VA_ARGS__)
#else
#define FD_DEBUG(msg, ...)
#define FD_WARNING(msg, ...)
#endif

#define FD_FATAL(msg, ...) FDLog(FD_LOG_LEVEL_FATAL, msg, __VA_ARGS__)

void FDAPI FDLog(unsigned char level, const char* message...);