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
		public MaterialList MaterialList { get; set; }

		public MaterialListControl()
		{
			InitializeComponent();

			this.MaterialList = new MaterialList();
			this.comboMaterialList.Sorted = true;
		}

		private void butMatAdd_Click(object sender, EventArgs e)
		{
			var mat = this.AddMaterial();

			propMaterial.SelectedObject = mat;

			viewMaterial.Invoke(new Action(() =>
			{
				viewMaterial.Material = mat;
				viewMaterial.Render();
			}));
		}

		private string GenMatName()
		{
			int i = 1;
			string name;

			do
			{
				name = string.Format("Material{0}", i);
			}
			while (this.MaterialList.ContainsID(name));

			return name;
		}

		public Material AddMaterial()
		{
			var mat = new Material(this.GenMatName(), this.MaterialList);

			this.MaterialList.Add(mat);
			this.comboMaterialList.Items.Add(mat.ID);
			this.comboMaterialList.SelectedIndex = this.comboMaterialList.Items.IndexOf(mat.ID);

			return mat;
		}
	}
}
