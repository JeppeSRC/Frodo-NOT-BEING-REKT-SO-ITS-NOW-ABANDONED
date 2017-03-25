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

        private String _name;

        public virtual String name { get { return _name; } set { _name = value; OnPropertyChanged(); } }

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] String name = "") {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

    }
}
