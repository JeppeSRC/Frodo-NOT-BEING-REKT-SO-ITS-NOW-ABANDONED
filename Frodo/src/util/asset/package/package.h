#pragma once

#include <fd.h>

#include <util/list.h>
#include <util/string.h>

enum FD_ASSET_TYPE {
	FD_ASSET_TYPE_UNKNOW,
	FD_ASSET_TYPE_BINARY,
	FD_ASSET_TYPE_TEXTURE2D,
	FD_ASSET_TYPE_TEXTURE3D,
	FD_ASSET_TYPE_MODEL,
	FD_ASSET_TYPE_TEXT
};

struct FD_ASSET {
	char name[32];

	uint32 size;

	uint64 data;

	FD_ASSET_TYPE type;
};

class FDAPI AssetPackage {
private:
	struct FD_HEADER {
		char signature[4]{ 'O', 'Y', 'M', '8' };
		uint16 version = 0x0101;
		char name[32]{ 0 };
		uint32 size;
		uint32 num_assets;
	} header;
private:
	String name;

	List<FD_ASSET> assets;

	bool writable;
public:
	AssetPackage(const String& name);
	AssetPackage() {}
	~AssetPackage();

	void AddAsset(const String& name, void* data, uint32 size, FD_ASSET_TYPE type);
	FD_ASSET GetAsset(const String& name);

	void Write(const String& filename);
	void Read(const String& filename);

	inline uint32 GetNumAssets() const { return header.num_assets; }
	inline uint32 GetSize() const { return header.size; }
	inline String GetName() const { return name; }
	inline const List<FD_ASSET>& GetAssets() const { return assets; }
};
