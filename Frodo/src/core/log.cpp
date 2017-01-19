#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

#define FD_LOG_COLOR_INFO 0b00001111
#define FD_LOG_COLOR_DEBUG 0b00001010
#define FD_LOG_COLOR_WARNING 0b00001110
#define FD_LOG_COLOR_FATAL 0b00001100

void FDLog(byte level, const char* message...) {

	HANDLE outhandle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO info;

	GetConsoleScreenBufferInfo(outhandle, &info);

	va_list args;
	va_start(args, message);

	switch (level) {
		case FD_LOG_LEVEL_INFO:
			SetConsoleTextAttribute(outhandle, FD_LOG_COLOR_INFO);
			printf("FD_INFO: ");
			vprintf(message, args);
			printf("\n");
			break;
		case FD_LOG_LEVEL_DEBUG:
			SetConsoleTextAttribute(outhandle, FD_LOG_COLOR_DEBUG);
			printf("FD_DEBUG: ");
			vprintf(message, args);
			printf("\n");
			break;
		case FD_LOG_LEVEL_WARNING:
			SetConsoleTextAttribute(outhandle, FD_LOG_COLOR_WARNING);
			printf("FD_WARNING: ");
			vprintf(message, args);
			printf("\n");
			break;
		case FD_LOG_LEVEL_FATAL:
			SetConsoleTextAttribute(outhandle, FD_LOG_COLOR_FATAL);
			printf("FD_FATAL: ");
			vprintf(message, args);
			printf("\n");
			break;
	}

	SetConsoleTextAttribute(outhandle, info.wAttributes);

	va_end(args);

}