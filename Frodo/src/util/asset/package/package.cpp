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
	asset.data = (unsigned long long)data;
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

void AssetPackage::Write(const String& filename) {

	FILE* file = fopen(*filename, "wb");
	
	unsigned int offset = assets.GetSize() * sizeof(FD_ASSET) + sizeof(FD_HEADER);

	for (size_t i = 0; i < assets.GetSize(); i++) {
		FD_ASSET asset = assets[i];

		fseek(file, offset, SEEK_SET);
		fwrite((const void*)asset.data, asset.size, 1, file);

		asset.data = (unsigned long long)offset;

		fseek(file, sizeof(FD_HEADER) + sizeof(FD_ASSET) * i, SEEK_SET);
		fwrite(&asset, sizeof(FD_ASSET), 1, file);


		offset += asset.size;
	}

	header.size = offset - assets.GetSize() * sizeof(FD_ASSET) + sizeof(FD_HEADER);
	header.num_assets = assets.GetSize();

	fseek(file, 0, SEEK_SET);
	fwrite(&header, sizeof(FD_HEADER), 1, file);

	fclose(file);
}

void AssetPackage::Read(const String& filename) {

	
	FILE* file = fopen(*filename, "rb");

	fseek(file, 0, SEEK_END);
	unsigned int size = (unsigned int)ftell(file);
	fseek(file, 0, SEEK_SET);

	unsigned char* data = new unsigned char[size];

	fread(data, size, 1, file);

	fclose(file);

	header = *(FD_HEADER*)data;

	

	name = String(header.name, 32);

	assets.Reserve(header.num_assets);

	FD_ASSET* a = (FD_ASSET*)(data + sizeof(FD_HEADER));

	for (unsigned int i = 0; i < header.num_assets; i++) {
		FD_ASSET asset = a[i];

		unsigned long long offset = asset.data;

		asset.data = (unsigned long long)new unsigned char[asset.size];
		memcpy((void*)asset.data, (const void*)(data + offset), asset.size);

		assets.Push_back(asset);

	
	}

}