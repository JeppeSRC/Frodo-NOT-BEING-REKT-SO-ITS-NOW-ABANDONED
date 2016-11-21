#include "package.h"

#include <memory>

AssetPackage::AssetPackage(const String& name) {
	this->name = name;
	assets.Reserve(128);

	size_t len = name.length < 32 ? name.length : 32;
	memcpy(header.name, *name.SubString(0, len), len);
}

AssetPackage::~AssetPackage() {
	
}

void AssetPackage::AddAsset(const String& name, void* data, unsigned int size, FD_ASSET_TYPE type) {
	FD_ASSET asset;
	memset(&asset, 0, sizeof(FD_ASSET));

	size_t len = name.length < 32 ? name.length : 32;
	memcpy(asset.name, *name.SubString(0, len), len);

	asset.size = size;
	asset.data = (unsigned long long*)data;
	asset.type = type;

	assets.Push_back(asset);
}

FD_ASSET AssetPackage::GetAsset(const String& name) {

	for (size_t i = 0; i < assets.GetSize(); i++) {
		FD_ASSET asset = assets[i];
		if (name == asset.name) return asset;
	}

	return FD_ASSET();
}

void AssetPackage::Flush(const String& filename) {

	FILE* file = fopen(*filename, "wb");
	
	unsigned int offset = assets.GetSize() * sizeof(FD_ASSET) + sizeof(FD_HEADER);

	for (size_t i = 0; i < assets.GetSize(); i++) {
		FD_ASSET asset = assets[i];

		fseek(file, sizeof(FD_HEADER) + sizeof(FD_ASSET) * i, SEEK_SET);
		fwrite(&asset, sizeof(FD_ASSET), 1, file);

		fseek(file, offset, SEEK_SET);
		fwrite((const void*)asset.data, asset.size, 1, file);

		offset += asset.size;
	}

	header.size = offset - assets.GetSize() * sizeof(FD_ASSET) + sizeof(FD_HEADER);
	header.num_assets = assets.GetSize();

	fseek(file, 0, SEEK_SET);
	fwrite(&header, sizeof(FD_HEADER), 1, file);

	fclose(file);
}