#include "cliasset.h"

#include <util/fileutils.h>

namespace FDCLI {

Asset::Asset(FD::Asset* asset, bool keepInstance) : CLIClass(asset, keepInstance) {}

Asset::Asset(FD::Asset* asset) : CLIClass(asset, false) {}

Asset::Asset() : CLIClass(new FD::Asset, false) {}


bool Asset::SetData(System::String^ filename) {
	FD::String fname = system_string_to_string(filename);

	uint_t fileSize = 0;

	instance->data = FD::FDReadBinaryFile(fname, &fileSize);
	instance->size = fileSize;

	if (instance->data == nullptr) {
		instance->size = 0;
		return false;
	}

	return true;
}
}
