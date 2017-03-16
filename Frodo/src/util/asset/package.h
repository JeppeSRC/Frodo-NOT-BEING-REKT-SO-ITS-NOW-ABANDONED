#pragma once

#include <util/string.h>
#include <util/list.h>

#include "asset.h"

namespace FD {

class FDAPI Package {
public:
	String name;
	List<Asset*> assets;

	void AddAsset(Asset* asset);
};

}