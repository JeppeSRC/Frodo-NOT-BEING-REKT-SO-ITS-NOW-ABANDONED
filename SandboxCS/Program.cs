using System;
using System.Data;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using FDCLI;

namespace SandboxCS {
    class Program {
        static void Main(string[] args) {

            /* Asset asset = new Asset();

             asset.name = "Asset";
             asset.folder = "FOlder";
             asset.type = FD_ASSET_TYPE.FD_ASSET_TYPE_RAW;
             asset.packageName = "package name not set";

             asset.SetData("kebab.txt");

             Package package = new Package();

             package.name = "Package";

             package.AddAsset(ref asset);

             AssetManager.ExportPackage("Test.pkg", ref package);*/

            AssetManager.LoadPackage("Test.pkg");

            
        }

    }
}
