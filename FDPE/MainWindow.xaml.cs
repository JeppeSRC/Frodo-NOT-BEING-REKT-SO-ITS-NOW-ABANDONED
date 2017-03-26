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

        private enum FILE_TYPE {
            UNKNOWN,
            //Image types
            JPG,
            PNG,
            TGA,
            BMP,

            //Model types
            OBJ,

            //Shader types
            HLSL,

            //Font types
            TTF,
            OTF,

            //Misc
            TXT,
            BIN,

            NUM_TYPES

        }

        private AssetFolder loadedPackagesRoot;
        private AssetFolder selectedFolder_newPackages = null;
        private AssetEntry selectedEntry_newPackages = null;
        private AssetFolder selectedFolder_loadedPackages = null;
        private AssetEntry selectedEntry_loadedPackages = null;

        private List<AssetFolder> selectedFolders_newPackages = new List<AssetFolder>();
        private List<AssetEntry> selectedEntries_newPackages = new List<AssetEntry>();
        private List<AssetFolder> selectedFolders_loadedPackages = new List<AssetFolder>();
        private List<AssetEntry> selectedEntries_loadedPackages = new List<AssetEntry>();

        private bool listSelect = false;

        private ObservableCollection<ExplorerBaseType> loadedPackages = new ObservableCollection<ExplorerBaseType>();
        private ObservableCollection<ExplorerBaseType> newPackages = new ObservableCollection<ExplorerBaseType>();

        public MainWindow() {
            InitializeComponent();

            loadedPackagesRoot = new AssetFolder("None");
            loadedPackages.Add(loadedPackagesRoot);

            tvLoadedPackages.ItemsSource = loadedPackages;
            tvNewPackages.ItemsSource = newPackages;
        }

        private bool LoadPackage(String filename) {
            String packageName = "";

            if (!AssetManager.LoadPackage(filename, ref packageName)) {
                return false;
            }

            Asset[] assets = AssetManager.GetAssetsByPackage(packageName);

            if (loadedPackagesRoot.name == "None") {
                loadedPackagesRoot.name = "All";
            }

            AssetFolder packageRoot = new AssetFolder(packageName);

            loadedPackages.Add(packageRoot);

            for (uint i = 0; i < assets.Length; i++) {
                loadedPackagesRoot.Add(new AssetEntry(assets[i]));

                FindFolder(packageRoot, assets[i].folder, true).Add(new AssetEntry(assets[i]));
            }

            for (int i = 0; i < packageRoot.folders.Count; i++) {
                if (packageRoot.folders[i] is AssetFolder) {
                    AssetFolder f = packageRoot.folders[i] as AssetFolder;

                    packageRoot.size += f.size;
                    packageRoot.numEntries += f.numEntries;
                    packageRoot.numFolders += f.numFolders;
                }


            }

            return true;
        }

        private AssetFolder CreateNewPackage(String name) {

            for (int i = 0; i < newPackages.Count; i++)
                if (newPackages[i].name == name) {
                    Log.WARNING("[FDPE] Package Already Exists: \"" + tbName.Text + "\"");
                    return newPackages[i] as AssetFolder;
                }

            AssetFolder root = new AssetFolder(name);
            newPackages.Add(root);

            Log.DEBUG("[FDPE] New Package: \"" + name + "\"");

            return root;
        }

        private AssetFolder CreateNewFolder(String name, AssetFolder parent) {
            return null;
        }

        private AssetFolder FindFolder(AssetFolder root, String folder, bool create) {

            String[] folderNames = folder.Split('/');

            String[] tmp = new String[folderNames.Length];

            for (int i = 0; i < tmp.Length; i++) {
                tmp[i] = folderNames[tmp.Length - i - 1];
            }

            return root.FindFolder(tmp, tmp.Length - 1, create);
        }

        private void tvSelectedItem_Changed(object sender, RoutedEventArgs e) {
            if ((TreeView)sender == tvNewPackages) {
                if (tvNewPackages.SelectedItem == null) return;
                selectedEntry_newPackages = null;
                selectedFolder_newPackages = null;
                selectedFolder_newPackages = tvNewPackages.SelectedItem is AssetFolder ? tvNewPackages.SelectedItem as AssetFolder : (selectedEntry_newPackages = (tvNewPackages.SelectedItem as AssetEntry)).parent;

                if (listSelect) {
                    if (tvNewPackages.SelectedItem is AssetEntry) { 
                        selectedEntries_newPackages.Add(selectedEntry_newPackages);
                        Log.DEBUG("[FDPE] List select new packages: added entry \"" + selectedEntry_newPackages.name + "\"");
                    } else { 
                        selectedFolders_newPackages.Add(selectedFolder_newPackages);
                        Log.DEBUG("[FDPE] List select new packages: added folder \"" + selectedFolder_newPackages.name + "\"");
                    }
                } else if (selectedEntries_newPackages.Count != 0 || selectedFolders_newPackages.Count != 0) {
                    selectedEntries_newPackages.Clear();
                    selectedFolders_newPackages.Clear();
                    Log.DEBUG("[FDPE] Cleared list new packages");
                }

            } else if ((TreeView)sender == tvLoadedPackages) {
                if (tvLoadedPackages.SelectedItem == null) return;
                selectedEntry_loadedPackages = null;
                selectedFolder_loadedPackages = null;
                selectedFolder_loadedPackages = tvLoadedPackages.SelectedItem is AssetFolder ? tvLoadedPackages.SelectedItem as AssetFolder : (selectedEntry_loadedPackages = (tvLoadedPackages.SelectedItem as AssetEntry)).parent;

                if (listSelect) {
                    if (tvLoadedPackages.SelectedItem is AssetEntry) {
                        selectedEntries_loadedPackages.Add(selectedEntry_loadedPackages);
                        Log.DEBUG("[FDPE] List select loaded packages: added entry \"" + selectedEntry_loadedPackages.name + "\"");
                    } else {
                        selectedFolders_loadedPackages.Add(selectedFolder_loadedPackages);
                        Log.DEBUG("[FDPE] List select loaded packages: added folder \"" + selectedFolder_loadedPackages.name + "\"");
                    }
                } else if (selectedEntries_loadedPackages.Count != 0 || selectedFolders_loadedPackages.Count != 0) {
                    selectedEntries_loadedPackages.Clear();
                    selectedFolders_loadedPackages.Clear();
                    Log.DEBUG("[FDPE] Cleared list loaded packages");
                }
            }
        }

        private void Key_Down(object sender, KeyEventArgs e) {
            if (e.Key == Key.LeftCtrl) {
                listSelect = true;
            }   
        }

        private void Key_Up(object sender, KeyEventArgs e) {
            if (e.Key == Key.LeftCtrl) {
                listSelect = false;
            }
        }

        private void btnClick_MoveSelected(object sender, RoutedEventArgs e) {

            if (selectedEntries_loadedPackages.Count == 0) {
                if (selectedEntry_loadedPackages != null) {
                    selectedEntries_loadedPackages.Add(selectedEntry_loadedPackages);
                } else if (selectedFolders_loadedPackages.Count == 0) {
                    if (selectedFolder_loadedPackages != null) selectedFolders_loadedPackages.Add(selectedFolder_loadedPackages);
                }
            }

            AssetFolder location = selectedFolder_newPackages;

            if (selectedFolders_loadedPackages.Count == 0 && selectedEntries_loadedPackages.Count == 0 || location == null) {
                Log.WARNING("[FDPE] Move Selected requires a destination package and at least one source asset/folder to be selected!");
                return;
            }


            if (chbKeepFolders.IsChecked == true) {
                location = location.GetRootParent();

                for (int i = 0; i < selectedFolders_loadedPackages.Count; i++) {
                    location.Add(selectedFolders_loadedPackages[i]);
                }

                for (int i = 0; i < selectedEntries_loadedPackages.Count; i++) {
                    AssetEntry c = selectedEntries_loadedPackages[i];

                    FindFolder(location, c.folder, true).Add(c);
                }
            } else {

                for (int i = 0; i < selectedFolders_loadedPackages.Count; i++) {
                    selectedFolders_loadedPackages[i].AddAllEntriesTo(location);
                }

                for (int i = 0; i < selectedEntries_loadedPackages.Count; i++) {
                    location.Add(selectedEntries_loadedPackages[i]);
                }

            }
        }

        private void btnClick_MoveAll(object sender, RoutedEventArgs e) {
            AssetFolder location = null;

            if (selectedFolder_newPackages == null) {
                Log.WARNING("[FDPE] Move All requires a destination package to be selected!");
                return;
            }

            if (chbKeepFolders.IsChecked == true) {
                location = selectedFolder_newPackages.GetRootParent();

                for (int i = 1; i < loadedPackages.Count; i++) {
                    AssetFolder curr = loadedPackages[i] as AssetFolder;
                    for (int j = 0; j < curr.folders.Count; j++) {
                        location.Add(curr.folders[j]);
                    }
                }

            } else {

                location = selectedFolder_newPackages;

                for (int i = 0; i < loadedPackagesRoot.folders.Count; i++)
                    location.Add(loadedPackagesRoot.folders[i]);
            }

            location.SortFolders();
        }

        private void btnClick_NewPackage(object sender, RoutedEventArgs e) {
            if (tbName.Text == "") return;
            CreateNewPackage(tbName.Text);
        }

        private void btnClick_NewFolder(object sender, RoutedEventArgs e) {
            if (tbName.Text == "" || selectedFolder_newPackages == null) return;
            selectedFolder_newPackages.Add(new AssetFolder(tbName.Text));
            selectedFolder_newPackages.SortFolders();
            Log.DEBUG("[FDPE] New Folder: \"" + tbName.Text + "\" in \"" + selectedFolder_newPackages.name + "\"");
        }

        private void btnClick_Remove(object sender, RoutedEventArgs e) {

            if (selectedEntry_newPackages != null) {
                Log.DEBUG("[FDPE] Removed Entry: \"" + selectedEntry_newPackages.name + "\"");
                selectedEntry_newPackages.parent.Remove(selectedEntry_newPackages);
            } else if (selectedFolder_newPackages != null) {
                if (selectedFolder_newPackages.parent == null) {
                    Log.DEBUG("[FDPE] Removed Package: \"" + selectedFolder_newPackages.name + "\"");
                    newPackages.Remove(selectedFolder_newPackages);
                } else {
                    Log.DEBUG("[FDPE] Removed Folder: \"" + selectedFolder_newPackages.name + "\"");
                    selectedFolder_newPackages.parent.Remove(selectedFolder_newPackages);
                }
            } else
                return;

            selectedEntry_newPackages = null;
           // selectedFolder_newPackages = null;
        }

        private void btnClick_OpenFiles(object sender, RoutedEventArgs e) {
            String[] files = OpenFileBrowser();

            if (files == null) return;

            for (int i = 0; i < files.Length; i++) {
                String path = files[i];
                if (!LoadPackage(files[i])) {
                    Asset asset = new Asset();
                    asset.name = GetNameFromPath(path);
                    asset.type = GetAssetTypeFromFileExension(GetFileExtension(path));
                    asset.SetData(path);

                    loadedPackagesRoot.Add(new AssetEntry(asset));
                }
            }
        }

        private void btnClick_Export(object sender, RoutedEventArgs e) {
            if (selectedFolder_newPackages == null) return;

            AssetFolder packageRoot = selectedFolder_newPackages.GetRootParent();

            Package package = new Package();
            package.name = packageRoot.name;

            List<AssetEntry> assets = new List<AssetEntry>();

            packageRoot.AddAllEntriesTo(ref assets);

            for (int i = 0; i < assets.Count; i++) {
                AssetEntry c = assets[i];

                c.UpdateFolderFromParent(true);
                package.AddAsset(ref c.asset);
            }

            String filename = SaveFileExplorer();

            if (filename == null) {
                Log.WARNING("[FDPE] Export failed: no location");
                return;
            }

            AssetManager.ExportPackage(filename, ref package);

            Log.DEBUG("[FDPE] Exported Package: \"" + package.name + "\" at \"" + filename + "\"");
        }

        private FD_ASSET_TYPE GetAssetTypeFromFileExension(String extension) {
            switch (GetFileTypeFromString(extension)) {
                case FILE_TYPE.JPG:
                case FILE_TYPE.PNG:
                case FILE_TYPE.TGA:
                case FILE_TYPE.BMP:
                    return FD_ASSET_TYPE.FD_ASSET_TYPE_TEXTURE2D;
                case FILE_TYPE.HLSL:
                    return FD_ASSET_TYPE.FD_ASSET_TYPE_SHADER;
                case FILE_TYPE.OBJ:
                    return FD_ASSET_TYPE.FD_ASSET_TYPE_MODEL;
                case FILE_TYPE.OTF:
                case FILE_TYPE.TTF:
                    return FD_ASSET_TYPE.FD_ASSET_TYPE_FONT;
                case FILE_TYPE.TXT:
                    return FD_ASSET_TYPE.FD_ASSET_TYPE_STRING;
                case FILE_TYPE.BIN:
                    return FD_ASSET_TYPE.FD_ASSET_TYPE_RAW;
            }

            return FD_ASSET_TYPE.FD_ASSET_TYPE_RAW;
        }

        private FILE_TYPE GetFileTypeFromString(String extension) {
            for (int i = 1; i < (int)FILE_TYPE.NUM_TYPES; i++) {
                if (((FILE_TYPE)i).ToString().Equals(extension, StringComparison.OrdinalIgnoreCase) == true) {
                    return (FILE_TYPE)i;
                }
            }

            return FILE_TYPE.UNKNOWN;
        } 

        private String[] OpenFileBrowser() {
            System.Windows.Forms.OpenFileDialog diag = new System.Windows.Forms.OpenFileDialog();
            diag.Multiselect = true;
            System.Windows.Forms.DialogResult res = diag.ShowDialog();

            if (res == System.Windows.Forms.DialogResult.OK) {
                return diag.FileNames;
            }

            return null;
        }

        private String SaveFileExplorer() {
            System.Windows.Forms.SaveFileDialog diag = new System.Windows.Forms.SaveFileDialog();
            System.Windows.Forms.DialogResult res = diag.ShowDialog();

            if (res == System.Windows.Forms.DialogResult.OK) {
                return diag.FileName;
            }

            return null;
        }

       private static String GetNameFromPath(String path) {
            String[] split = path.Split('\\', '.');

            return split[split.Length - 2];
        }

        private static String GetFileExtension(String file) {
            String[] split = file.Split('.');

            return split[split.Length - 1];
        }
    }
}
