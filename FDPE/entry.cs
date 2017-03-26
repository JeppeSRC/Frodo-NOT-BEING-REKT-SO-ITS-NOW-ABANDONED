using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using FDCLI;

namespace FDPE {
    public class AssetEntry : ExplorerBaseType {

        public Asset asset;

        public override String name {
            get {
                return asset.name;
            }

            set {
                asset.name = value;
                OnPropertyChanged();
            }
        }

        public override ulong size {
            get {
                return asset.size;
            }
        }

        public String packageName {
            get {
                return asset.packageName;
            }

            set {
                asset.packageName = value;
                OnPropertyChanged();
            }
        }

        public String folder {
            get {
                return asset.folder;
            }

            set {
                asset.folder = value;
                OnPropertyChanged();
            }
        }

        public FD_ASSET_TYPE type {
            get {
                return asset.type;
            }

            set {
                asset.type = value;
                OnPropertyChanged();
            }
        }

        public String typeAsString {
            get {
                switch (asset.type) {
                    case FD_ASSET_TYPE.FD_ASSET_TYPE_RAW:
                        return "RAW";
                    case FD_ASSET_TYPE.FD_ASSET_TYPE_STRING:
                        return "STRING";
                    case FD_ASSET_TYPE.FD_ASSET_TYPE_FONT:
                        return "FONT";
                    case FD_ASSET_TYPE.FD_ASSET_TYPE_TEXTURE2D:
                        return "TEXTURE2D";
                    case FD_ASSET_TYPE.FD_ASSET_TYPE_TEXTURECUBE:
                        return "TEXTURECUBE";
                    case FD_ASSET_TYPE.FD_ASSET_TYPE_MODEL:
                        return "MODEL";
                }

                return "UNKNOWN";
            }
        }

        public AssetEntry(Asset asset) {
            this.asset = asset;
        }

        public void UpdateFolderFromParent(bool skipAbsoluteRoot) {
            folder = parent.GetFolderPath(skipAbsoluteRoot);
        }
    }
}
