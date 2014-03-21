using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace MaterialViewControl
{
	[Serializable]
	public partial class TextureManagerView : UserControl
	{
		public Dictionary<string, string> TextureDB
		{
			set
			{
				this.listTexList.Items.Clear();
				foreach (var item in value)
				{
					var listItem = this.listTexList.Items.Add(item.Key);
					listItem.SubItems.Add(item.Value);
				}
			}
			get
			{
				Dictionary<string, string> dict = new Dictionary<string, string>();
				foreach (ListViewItem item in this.listTexList.Items)
				{
					dict.Add(item.SubItems[0].Text, item.SubItems[1].Text);
				}
				return dict;
			}
		}
		public string TextureDirectory
		{
			set
			{
				this.textTexRootDir.Text = value;
			}
			get
			{
				return this.textTexRootDir.Text;
			}
		}
		public string SelectedTextureID
		{
			set
			{
				this.listTexList.SelectedItems.Clear();
				var item = this.listTexList.Items.Find(value, false);
				if (item != null && item.Length > 0)
					item[0].Selected = true;
			}
			get
			{
				if (this.listTexList.SelectedItems.Count > 0)
					return this.listTexList.SelectedItems[0].Text;
				return null;
			}
		}

		//[Category("Property Changed")]
		//public event EventHandler<string> SelectedTextureChanged;

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
					this.textTexRootDir.Text = dialog.SelectedPath;
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
					this.ActionEditTexture();
					break;

				case ListAction.Delete:
					if (this.listTexList.SelectedItems.Count > 0)
						this.listTexList.Items.Remove(listTexList.SelectedItems[0]);
					break;

				case ListAction.Clear:
					this.listTexList.Items.Clear();
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
					item.SubItems.Add(dialog.TextureFile);

					this.listTexList.Items.Add(item);
				}
			}
		}

		private void ActionEditTexture()
		{
			if (this.listTexList.SelectedItems.Count == 0)
				return;

			var item = this.listTexList.SelectedItems[0];
			using (var dialog = new TextureEditDialog(this.textTexRootDir.Text))
			{
				dialog.TextureID = item.SubItems[0].Text;
				dialog.TextureFile = item.SubItems[1].Text;
				if (dialog.ShowDialog() == DialogResult.OK)
				{
					item.SubItems[0].Text = dialog.TextureID;
					item.SubItems[1].Text = dialog.TextureFile;
				}
			}
		}

		private void listTexList_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
		{
			if (e.Item == null)
			{
				this.pictureTextureImage.Image = null;
				//if (this.SelectedTextureChanged != null)
				//	this.SelectedTextureChanged(this, null);
				return;
			}

			var file = Path.Combine(this.textTexRootDir.Text, e.Item.SubItems[1].Text);
			if (File.Exists(file))
				this.pictureTextureImage.Image = Extensions.LoadImageFile(file);
			else
				this.pictureTextureImage.Image = null;

			//this.SelectedTextureChanged(this, e.Item.SubItems[0].Text);
		}
	}
}
