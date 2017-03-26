using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FDPE {
    public class AssetFolder : ExplorerBaseType {

        public ObservableCollection<ExplorerBaseType> folders { get; set; }

        private String _name;
        private uint _numEntries;
        private uint _numFolders;
        private ulong _totalSize;

        public uint numEntries { get { return _numEntries; } set { _numEntries = value; OnPropertyChanged(); } }
        public uint numFolders { get { return _numFolders; } set { _numFolders = value; OnPropertyChanged(); } }
        public override ulong size { get { return _totalSize; } set { _totalSize = value; OnPropertyChanged(); } }
        public override String name { get { return _name; } set { _name = value; OnPropertyChanged(); } }

        public AssetFolder(String name) {
            this.name = name;
            parent = null;
            folders = new ObservableCollection<ExplorerBaseType>();

            numEntries = 0;
            numFolders = 0;
            size = 0;
        }

        private void UpdateProperties() {
            size = 0;

            for (int i = 0; i < folders.Count; i++) {
                size += folders[i].size;
            }

            parent?.UpdateProperties();
        }

        public void Add(ExplorerBaseType asset) {
            if (asset is AssetFolder)
                Add(asset as AssetFolder);
            else
                Add(asset as AssetEntry);
        } 

        public void Add(AssetFolder folder) {
            if (Exists(folder.name)) {
                FDCLI.Log.WARNING("[FDPE] Folder Alrady Exists: \"" + folder.name+ "\"" + " In " + "\"" + name + "\"");
                return;
            }
            folder.parent = this;
            folders.Add(folder);
            numFolders++;
            size += folder.size;
            parent?.UpdateProperties();
        }

        public void Add(AssetEntry entry) {
            if (Exists(entry.name)) {
                FDCLI.Log.WARNING("[FDPE] Asset Alrady Exists: \"" + entry.name + "\"" + " In " + "\"" + name + "\"");
                return;
            }
            entry.parent = this;
            folders.Add(entry);
            numEntries++;
            size += entry.size;
            parent?.UpdateProperties();
        }

        public void Remove(ExplorerBaseType asset) {
            if (asset is AssetFolder)
                Remove(asset as AssetFolder);
            else
                Remove(asset as AssetEntry);
        }

        public void Remove(AssetFolder folder) {
            folders.Remove(folder);
            numFolders--;
            size -= folder.size;
            parent?.UpdateProperties();
        }

        public void Remove(AssetEntry entry) {
            folders.Remove(entry);
            numEntries--;
            size -= entry.size;
            parent?.UpdateProperties();
        }

        public void AddAllEntriesTo(AssetFolder root) {
            for (int i = 0; i < folders.Count; i++) {
                if (folders[i] is AssetEntry) {
                    root.Add(folders[i]);
                } else {
                    (folders[i] as AssetFolder).AddAllEntriesTo(root);
                }
            }
        }

        public void AddAllEntriesTo(ref List<AssetEntry> list) {
            for (int i = 0; i < folders.Count; i++) {
                ExplorerBaseType c = folders[i];
                if (c is AssetEntry) list.Add(c as AssetEntry);
                else (c as AssetFolder).AddAllEntriesTo(ref list);
            }
        }

        public bool Exists(String name) {
            for (int i = 0; i < folders.Count; i++)
                if (folders[i].name == name) return true;

            return false;
        }

        public AssetFolder FindFolder(String[] folder, int index, bool create) {

            for (int i = 0; i < folders.Count; i++) {
                if (folders[i] is AssetFolder) {
                    AssetFolder curr = folders[i] as AssetFolder;
                    if (curr.name == folder[index]) {
                        if (index == 0) return curr;

                        return curr.FindFolder(folder, index - 1, create);
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

        public AssetFolder GetRootParent() {
            if (parent == null)
                return this;
            else
                return parent.GetRootParent();
        }

        public void SortFolders() {
            List<ExplorerBaseType> entries = new List<ExplorerBaseType>();
            List<ExplorerBaseType> folders = new List<ExplorerBaseType>();

            for (int i = 0; i < this.folders.Count; i++) {
                ExplorerBaseType c = this.folders[i];
                if (c is AssetFolder) {
                    folders.Add(c);
                    (c as AssetFolder).SortFolders();
                } else {
                    entries.Add(c);
                }
            }

            this.folders.Clear();

            for (int i = 0; i < folders.Count; i++) {
                this.folders.Add(folders[i]);
            }

            for (int i = 0; i < entries.Count; i++) {
                this.folders.Add(entries[i]);
            }
        }

        public String GetFolderPath(bool skipAbsoluteRoot) {
            if (skipAbsoluteRoot == true) {
                if (parent.parent == null) return name;
            }
            if (parent == null) return name;

            return parent.GetFolderPath(skipAbsoluteRoot) + "/";
        }
    }
}
