#include "clipackage.h"

namespace FDCLI {

Package::Package(FD::Package* package, bool keepInstance) : CLIClass(package, keepInstance) { }

Package::Package(FD::Package* package) : CLIClass(package, false) { }

Package::Package() : CLIClass(new FD::Package, false) { }

void Package::AddAsset(Asset^% asset) {
	instance->AddAsset(asset->GetHandle());
}

}