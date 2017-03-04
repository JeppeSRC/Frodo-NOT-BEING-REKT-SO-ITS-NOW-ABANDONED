#pragma once

#include <fd.h>
#include <util/map.h>
#include "asset.h"
#include "package.h"

#define FD_ASSETMANAGER_VERSION 0x0001

class FDAPI AssetManager {
private:
	struct PACKAGE_HEADER {
		char signature[4]{'P', 'H', 'D', 'R'};
		uint16 version;
		uint32 nameLength;
		uint64 nameDataOffset;
		uint32 numberOfAssets;
	};

	struct ASSET_DIRECTORY_ENTRY {
		uint32 nameLength;
		uint64 nameDataOffset;
		uint32 folderLength;
		uint64 folderDataOffset;
		FD_ASSET_TYPE type;
		uint64 size;
		uint64 dataOffset;
	};

	static bool ValidatePackageHeader(PACKAGE_HEADER* hdr, const String& filename);

private:
	static List<Asset*> assets;

public:
	static void LoadPackage(const Package* package);
	static bool LoadPackage(const String& filename);
	static void UnloadPackage(const String& packageName);
	static List<Asset*> GetAssetsByFolder(const String& name);
	static List<Asset*> GetAssetsByType(FD_ASSET_TYPE type);
	static List<Asset*> GetAssetsByPackage(const String& name);
	static Asset* GetAsset(const String& name);
	static bool ExportPackage(const String& filename, const Package* package);
	static Package* MakePackage(const String& name, bool exportPackage);
	
};
