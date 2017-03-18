#pragma once

#include <clicore.h>
#include <util/asset/package.h>
#include "cliasset.h"

namespace FDCLI {

public ref class Package : public CLIClass<FD::Package> {
public:

	property System::String^ name {
		System::String^ get() {
			return string_to_system_string(instance->name);
		}

		void set(System::String^ string) {
			instance->name = system_string_to_string(string);
		}
	}

	Package(FD::Package* package, bool keepInstance);
	Package(FD::Package* package);
	Package();

	void AddAsset(Asset^% asset);
};

}