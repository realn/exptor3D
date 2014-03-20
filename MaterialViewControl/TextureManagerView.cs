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
	public partial class TextureManagerView : UserControl
	{
		private enum ListAction
		{
			Add,
			Edit,
			Delete,
			Clear,
		}

		public TextureManagerView()
		{
			InitializeComponent();

			this.comboAction.SetEnum<ListAction>();
		}

		private void butBrowseRoot_Click(object sender, EventArgs e)
		{
			using (var dialog = new FolderBrowserDialog())
			{
				dialog.SelectedPath = this.textTexRootDir.Text;
				if (dialog.ShowDialog() == DialogResult.OK)
				{
					this.textTexRootDir.Text = dialog.SelectedPath;
				}
			}
		}

		private void butAction_Click(object sender, EventArgs e)
		{
			switch (this.comboAction.GetSelectedEnum<ListAction>())
			{
				case ListAction.Add:
					this.ActionAddTexture();
					break;
				case ListAction.Edit:
					break;
				case ListAction.Delete:
					break;
				case ListAction.Clear:
					break;
				default:
					break;
			}
		}

		private void ActionAddTexture()
		{
			using (var dialog = new TextureEditDialog(this.textTexRootDir.Text))
			{
				if (dialog.ShowDialog() == DialogResult.OK)
				{
					var item = new ListViewItem();
					item.Text = dialog.TextureID;
					//item.SubItems.Add(dialog.TextureID);
					item.SubItems.Add(dialog.TextureFile);

					this.listTexList.Items.Add(item);
				}
			}
		}
	}
}
