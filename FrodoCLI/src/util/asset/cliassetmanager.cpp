#include "cliassetmanager.h"

namespace FDCLI {

void AssetManager::LoadPackage(Package^% package) {
	FD::AssetManager::LoadPackage(package->GetHandle());
}

bool AssetManager::LoadPackage(System::String^ filename) {
	return FD::AssetManager::LoadPackage(system_string_to_string(filename));
}

void AssetManager::UnloadPackage(System::String^ packageName) {
	FD::AssetManager::UnloadPackage(system_string_to_string(packageName));
}

array<Asset^>^ AssetManager::GetAssetsByFolder(System::String^ name) {
	FD::List<FD::Asset*> assets = FD::AssetManager::GetAssetsByFolder(system_string_to_string(name));

	uint_t size = assets.GetSize();

	array<Asset^>^ data = gcnew array<Asset^>(size);

	for (uint_t i = 0; i < size; i++) {
		data[i] = gcnew Asset(assets[i]);
	}

	return data;
}

array<Asset^>^ AssetManager::GetAssetsByType(FD_ASSET_TYPE type) {
	FD::List<FD::Asset*> assets = FD::AssetManager::GetAssetsByType((FD::FD_ASSET_TYPE)type);

	uint_t size = assets.GetSize();

	array<Asset^>^ data = gcnew array<Asset^>(size);

	for (uint_t i = 0; i < size; i++) {
		data[i] = gcnew Asset(assets[i]);
	}

	return data;
}

array<Asset^>^ AssetManager::GetAssetsByPackage(System::String^ name) {
	FD::List<FD::Asset*> assets = FD::AssetManager::GetAssetsByPackage(system_string_to_string(name));

	uint_t size = assets.GetSize();

	array<Asset^>^ data = gcnew array<Asset^>(size);

	for (uint_t i = 0; i < size; i++) {
		data[i] = gcnew Asset(assets[i]);
	}

	return data;
}

Asset^ AssetManager::GetAsset(System::String^ name) {
	FD::Asset* asset = FD::AssetManager::GetAsset(system_string_to_string(name));
	return gcnew Asset(asset, true);
}

bool AssetManager::ExportPackage(System::String^ filename, Package^% package) {
	return FD::AssetManager::ExportPackage(system_string_to_string(filename), package->GetHandle());
}

Package^ AssetManager::MakePackage(System::String^ name) {
	FD::Package* package = FD::AssetManager::MakePackage(system_string_to_string(name));
	return gcnew Package(package);
}

}
