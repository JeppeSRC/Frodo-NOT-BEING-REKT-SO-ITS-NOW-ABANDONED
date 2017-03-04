#include "assetmanager.h"
#include <util/fileutils.h>

List<Asset*> AssetManager::assets;

bool AssetManager::ValidatePackageHeader(PACKAGE_HEADER* hdr, const String& filename) {
	if (memcmp("PHDR", hdr->signature, 4) != 0) {
		FD_FATAL("[AssetManager] Failed to validate header in \"%s\": Invalid signature<%c%c%c%c>", *filename, hdr->signature[0], hdr->signature[1], hdr->signature[2], hdr->signature[3]);
		return false;
	}

	if (hdr->version != FD_ASSETMANAGER_VERSION) {
		FD_FATAL("[AssetManager] Failed to validate header in \"%s\": Invalid version<0x%04x>", *filename, hdr->version);
		return false;
	}

	return true;
}

void AssetManager::LoadPackage(const Package* package) {
	uint_t size = package->assets.GetSize();

	for (uint_t i = 0; i < size; i++)
		assets.Push_back(package->assets.Get(i));
}

bool AssetManager::LoadPackage(const String& filename) {
	
	uint_t fileSize = 0;

	byte* data = FDReadBinaryFile(filename, &fileSize);

	if (!data) {
		FD_FATAL("[AssetManager] Failed to open file \"%s\"", *filename);
		return false;
	}

	PACKAGE_HEADER* hdr = (PACKAGE_HEADER*)data;

	if (!ValidatePackageHeader(hdr, filename)) {
		return false;
	}

	String packageName((char*)(data + hdr->nameDataOffset), hdr->nameLength);

	
}

void AssetManager::UnloadPackage(const String& packageName) {
	List<Asset*> tmp;

	uint_t size = assets.GetSize();

	for (uint_t i = 0; i < size; i++) {
		Asset* a = assets[i];
		if (a->packageName == packageName) tmp.Push_back(a);
	}

	size = tmp.GetSize();

	for (uint_t i = 0; i < size; i++) {
		delete assets.Remove(tmp[i]);
	}
}

List<Asset*> AssetManager::GetAssetsByFolder(const String& name) {
	List<Asset*> tmp;

	uint_t size = assets.GetSize();

	for (uint_t i = 0; i < size; i++) {
		Asset* a = assets[i];
		if (a->folder == name) tmp.Push_back(a);
	}

	return tmp;
}

List<Asset*> AssetManager::GetAssetsByType(FD_ASSET_TYPE type) {
	List<Asset*> tmp;

	uint_t size = assets.GetSize();

	for (uint_t i = 0; i < size; i++) {
		Asset* a = assets[i];
		if (a->type == type) tmp.Push_back(a);
	}

	return tmp;
}

List<Asset*> AssetManager::GetAssetsByPackage(const String& name) {
	List<Asset*> tmp;

	uint_t size = assets.GetSize();

	for (uint_t i = 0; i < size; i++) {
		Asset* a = assets[i];
		if (a->packageName == name) tmp.Push_back(a);
	}

	return tmp;
}

Asset* AssetManager::GetAsset(const String& name) {
	uint_t size = assets.GetSize();

	Asset* a = nullptr;

	for (uint_t i = 0; i < size; i++)
		if ((a = assets[i])->name == name) return a;

	return a;
}

bool AssetManager::ExportPackage(const String& filename, const Package* package) {
	FILE* file = fopen(*filename, "wb");

	if (file) {

		PACKAGE_HEADER hdr;

		hdr.version = FD_ASSETMANAGER_VERSION;
		hdr.nameLength = (uint32)package->name.length;
		hdr.numberOfAssets = (uint32)package->assets.GetSize();

		uint64 assetDirectorySize = hdr.numberOfAssets * sizeof(ASSET_DIRECTORY_ENTRY);
		uint64 assetDirectoryStart = sizeof(PACKAGE_HEADER);
		uint64 dataOffset = assetDirectorySize + assetDirectoryStart;

		FDWriteFile(file, &hdr, sizeof(PACKAGE_HEADER));
		hdr.nameDataOffset = dataOffset;
		FDWriteFile(file, *package->name, hdr.nameLength, &dataOffset);

		uint_t size = package->assets.GetSize();

		for (uint_t i = 0; i < size; i++) {
			ASSET_DIRECTORY_ENTRY e;

			const Asset& a = *assets[i];

			e.type = a.type;
			e.size = a.size;
			e.nameLength = a.name.length;
			e.folderLength = a.folder.length;


			e.nameDataOffset = dataOffset;
			FDWriteFile(file, *a.name, e.nameLength, &dataOffset);

			e.folderDataOffset = dataOffset;
			FDWriteFile(file, *a.folder, e.folderLength, &dataOffset);

			e.dataOffset = dataOffset;
			FDWriteFile(file, a.data, e.size, &dataOffset);

			FDWriteFile(file, &e, sizeof(ASSET_DIRECTORY_ENTRY), sizeof(ASSET_DIRECTORY_ENTRY) * i + assetDirectoryStart);
		}

		return true;
	}

	FD_FATAL("[AssetManager] Failed to open file \"%s\"", *filename);

	return false;
}

Package* AssetManager::MakePackage(const String& name, bool exportPackage) {
	Package* p = new Package;

	p->name = name;
	uint_t size = assets.GetSize();
	
	for (uint_t i = 0; i < size; i++)
		p->AddAsset(assets[i]);

	if (exportPackage) ExportPackage(name, p);

	return p;
}