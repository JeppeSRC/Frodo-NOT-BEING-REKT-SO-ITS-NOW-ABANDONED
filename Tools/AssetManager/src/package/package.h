#pragma once

#include <fda.h>

#include "format.h"


class FDAPI AssetPackage {
private:
	FD_ASSET_PACKAGE header;

	bool writable;
public:
	AssetPackage(const char* name);
	~AssetPackage();

	void AddAsset(const char* name, void* data, unsigned int size, FD_ASSET_TYPE type);
	void* GetAsset(const char* name);

	void Flush();

	inline unsigned int GetNumAssets() const { return header.num_files; }
	inline unsigned int GetSize() const { return header.size; }

};
