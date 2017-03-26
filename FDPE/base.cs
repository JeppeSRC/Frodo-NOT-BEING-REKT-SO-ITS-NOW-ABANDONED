using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace FDPE {
    public class ExplorerBaseType : INotifyPropertyChanged {
        public AssetFolder parent = null;

        public virtual String name { get; set; }
        public virtual ulong size { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] String name = "") {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

    }
}
