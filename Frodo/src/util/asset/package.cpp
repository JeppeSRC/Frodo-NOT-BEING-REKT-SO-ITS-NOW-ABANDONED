#include "package.h"

void Package::AddAsset(Asset* asset) {
	Asset* a = new Asset(asset);
	a->packageName = name;
	assets.Push_back(a);
}