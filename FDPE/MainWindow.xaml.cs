using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using FDCLI;

namespace FDPE {

    public partial class MainWindow : Window {
        private AssetFolder root;

        private List<ExplorerBaseType> loadedPackages = new List<ExplorerBaseType>();

        public MainWindow() {
            InitializeComponent();

            LoadPackage("Test.pkg");

            tvLoadedPackages.ItemsSource = loadedPackages;
        }

        private bool LoadPackage(String filename) {
            String packageName = "";

           if (!AssetManager.LoadPackage(filename, ref packageName)) {
                return false;
            }

            Asset[] assets = AssetManager.GetAssets();

            if (root == null) {
                root = new AssetFolder("All");
                loadedPackages.Add(root);
            }

            AssetFolder packageRoot = new AssetFolder(packageName);

            loadedPackages.Add(packageRoot);

            for (uint i = 0; i < assets.Length; i++) {
                root.Add(new AssetEntry(assets[i]));

                FindFolder(packageRoot, assets[i].folder, true).Add(new AssetEntry(assets[i]));
            }

            for (int i = 0; i < packageRoot.folders.Count; i++) {
                if (packageRoot.folders[i] is AssetFolder) {
                    AssetFolder f = packageRoot.folders[i] as AssetFolder;

                    packageRoot.totalSize += f.totalSize;
                    packageRoot.numEntries += f.numEntries;
                    packageRoot.numFolders += f.numFolders;
                }
                
                
            }

            return true;
        }

        private AssetFolder FindFolder(AssetFolder root, String folder, bool create) {

            String[] folderNames = folder.Split('/');

            String[] tmp = new String[folderNames.Length];

            for (int i = 0; i < tmp.Length; i++) {
                tmp[i] = folderNames[tmp.Length - i - 1];
            }

            return root.FindFolder(tmp, tmp.Length-1, create);
        }
    }

    public class ExplorerBaseType {


    }

    public class AssetEntry : ExplorerBaseType {

        private Asset asset;

        public String name {
            get {
                return asset.name;
            }
        }

        public ulong size {
            get {
                return asset.size;
            }
        }
        
        public String packageName {
            get {
                return asset.packageName;
            }
        }

        public String type {
            get {
                switch(asset.type) {
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
                }

                return "UNKNOWN";
            }
        }
        
        public AssetEntry(Asset asset) {
            this.asset = asset;
        }
    }

    public class AssetFolder : ExplorerBaseType {

        public String name { get; set; }

        public AssetFolder parentFolder;

        public ObservableCollection<ExplorerBaseType> folders { get; set; }

        public uint numEntries { get; set; }
        public uint numFolders { get; set; }
        public ulong totalSize { get; set; }

        public AssetFolder(String name) {
            this.name = name;
            parentFolder = null;
            folders = new ObservableCollection<ExplorerBaseType>();

            numEntries = 0;
            numFolders = 0;
            totalSize = 0;
        }

        public void Add(AssetFolder folder) {
            folder.parentFolder = this;
            folders.Add(folder);
            numFolders++;
            totalSize += folder.totalSize;
        }

        public void Add(AssetEntry entry) {
            folders.Add(entry);
            numEntries++;
            totalSize += entry.size;
        }

        public AssetFolder FindFolder(String[] folder, int index, bool create) {
            
            for (int i = 0; i < folders.Count; i++) {
                if (folders[i] is AssetFolder) {
                    AssetFolder curr = folders[i] as AssetFolder;
                    if (curr.name == folder[index]) {
                        if (index == 0) return curr;

                        return curr.FindFolder(folder, index-1, create);
                    }
                }
            }

            if (create) {
                Add(new AssetFolder(folder[index]));
                if (index == 0) {
                    return folders[folders.Count - 1] as AssetFolder;
                } else {
                    (folders[folders.Count - 1] as AssetFolder).FindFolder(folder, index - 1, create);
                }
            }

            return null;
        }
    }
}
