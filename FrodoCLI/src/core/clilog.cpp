#include "clilog.h"

#include <core/log.h>

namespace FDCLI {

void Log::FDLog(byte level, System::String^ message) {
	FD::FDLog(level, "%s", *system_string_to_string(message));
}

void Log::INFO(System::String^ message) {
	FDLog(FD_LOG_LEVEL_INFO, message);
}

void Log::DEBUG(System::String^ message) {
	FDLog(FD_LOG_LEVEL_DEBUG, message);
}

void Log::WARNING(System::String^ message) {
	FDLog(FD_LOG_LEVEL_WARNING, message);
}

void Log::FATAL(System::String^ message) {
	FDLog(FD_LOG_LEVEL_FATAL, message);
}

}
