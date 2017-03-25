#pragma once

#include <clicore.h>
#include <core/fdtypes.h>

namespace FDCLI {

	public ref class Log {
	private:
		static void FDLog(byte level, System::String^ message);

	public:
		static void INFO(System::String^ message);
		static void DEBUG(System::String^ message);
		static void WARNING(System::String^ message);
		static void FATAL(System::String^ message);
	};

}