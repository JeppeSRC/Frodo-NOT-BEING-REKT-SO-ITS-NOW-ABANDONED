#pragma once

#include <clicore.h>
#include <util/asset/asset.h>

namespace FDCLI {

public enum class FD_ASSET_TYPE {
	FD_ASSET_TYPE_RAW,
	FD_ASSET_TYPE_STRING,
	FD_ASSET_TYPE_FONT,
	FD_ASSET_TYPE_SHADER,

	FD_ASSET_TYPE_TEXTURE2D,
	FD_ASSET_TYPE_TEXTURECUBE,

	FD_ASSET_TYPE_MODEL
};


public ref class Asset : CLIClass<FD::Asset> {
public:
	property System::String^ name {
		System::String^ get (){
			return string_to_system_string(instance->name);
		}

		void set(System::String^ string) {
			instance->name = system_string_to_string(string);
		}
	}

	property System::String^ folder {
		System::String^ get() {
			return string_to_system_string(instance->folder);
		}

		void set(System::String^ string) {
			instance->folder = system_string_to_string(string);
		}
	}

	property uint64 size {
		uint64 get() {
			return instance->size;
		}

		void set(uint64 value) {
			instance->size = value;
		}
	}
	
	property FD_ASSET_TYPE type {
		FD_ASSET_TYPE get() {
			return (FD_ASSET_TYPE)instance->type;
		}

		void set(FD_ASSET_TYPE type) {
			instance->type = (FD::FD_ASSET_TYPE)type;
		}
	}

	property System::String^ packageName {
		System::String^ get (){
			return string_to_system_string(instance->packageName);
		}

		void set(System::String^ string) {
			instance->packageName = system_string_to_string(string);
		}
	}

	Asset(FD::Asset* asset, bool keepInstance);
	Asset(FD::Asset* asset);
	Asset();

	bool SetData(System::String^ filename);
};

}