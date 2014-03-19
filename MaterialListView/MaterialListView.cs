using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MaterialViewControl
{
	public partial class MaterialListControl : UserControl
	{
		public List<Material> MaterialList { get; set; }

		public MaterialListControl()
		{
			InitializeComponent();

			this.MaterialList = new List<Material>();
		}

		private void butMatAdd_Click(object sender, EventArgs e)
		{
			var mat = this.AddMaterial("Test mat");

			propMaterial.SelectedObject = mat;

			viewMaterial.Invoke(new Action(() =>
			{
				viewMaterial.Material = mat;
				viewMaterial.Render();
			}));
		}

		public Material AddMaterial(string name)
		{
			var mat = new Material("Test Material");

			this.MaterialList.Add(mat);
			comboMaterialList.Items.Add(mat.Name);

			return mat;
		}
	}
}
