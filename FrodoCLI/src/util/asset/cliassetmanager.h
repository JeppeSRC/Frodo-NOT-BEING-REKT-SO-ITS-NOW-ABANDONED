#pragma once

#include <clicore.h>
#include <util/asset/assetmanager.h>
#include "clipackage.h"

namespace FDCLI {

public ref class AssetManager : public CLIClass<FD::AssetManager> {
public:

	static void LoadPackage(Package^% package);
	static bool LoadPackage(System::String^ filename);
	static bool LoadPackage(System::String^ filename, System::String^% packageName);
	static void UnloadPackage(System::String^ packageName);
	static array<Asset^>^ GetAssetsByFolder(System::String^ name);
	static array<Asset^>^ GetAssetsByType(FD_ASSET_TYPE type);
	static array<Asset^>^ GetAssetsByPackage(System::String^ name);
	static array<Asset^>^ GetAssets();
	static Asset^ GetAsset(System::String^ name);
	static bool ExportPackage(System::String^ filename, Package^% package);
	static Package^ MakePackage(System::String^ name);
};


}

