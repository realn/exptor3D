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
	public partial class ModelForm : Form
	{
		private TreeNode mainNode;

		public ModelForm()
		{
			InitializeComponent();
		}

		protected override void OnLoad(EventArgs e)
		{
			base.OnLoad(e);

			this.mainNode = this.treeModelContent.Nodes.Add("NewModel");
			
		}

		public void LoadModel(string filename)
		{
			mainNode.Nodes.Add("NewObject");
		}
	}
}
