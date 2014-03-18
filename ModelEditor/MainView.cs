using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ModelEditor
{
    public partial class MainView : Form
    {
        public MainView()
        {
            InitializeComponent();
        }

        private void menuRefresh_Click(object sender, EventArgs e)
        {
            this.modelControl.Render();
        }
    }
}
