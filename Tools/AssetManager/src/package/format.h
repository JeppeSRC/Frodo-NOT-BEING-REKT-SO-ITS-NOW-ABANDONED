#pragma once

enum FD_ASSET_TYPE {
	FD_ASSET_TYPE_UNKNOW,
	FD_ASSET_TYPE_BINARY,
	FD_ASSET_TYPE_TEXTURE2D,
	FD_ASSET_TYPE_TEXTURE3D,
	FD_ASSET_TYPE_MODEL,
	FD_ASSET_TYPE_TEXT
};

struct FD_ASSET_PACKAGE {
	char name[32];

	unsigned int size;

	unsigned int num_files;

	unsigned int data_offset;
};

struct FD_ASSET_HEADER {
	char name[32];

	unsigned int size;

	unsigned int data_offset;

	FD_ASSET_TYPE type;
};
