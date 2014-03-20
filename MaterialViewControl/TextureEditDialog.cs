using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MaterialViewControl
{
	public partial class TextureEditDialog : Form
	{
		public string RootDirectory { get; private set; }

		public string TextureID
		{
			set
			{
				this.textID.Text = value;
			}
			get
			{
				return textID.Text;
			}
		}

		public string TextureFile
		{
			set
			{
				this.textFile.Text = value;
			}
			get
			{
				return textFile.Text;
			}
		}

		public TextureEditDialog( string rootDirectory )
		{
			this.RootDirectory = rootDirectory;
			InitializeComponent();
		}

		private void textFile_TextChanged(object sender, EventArgs e)
		{
			string file = Path.Combine(this.RootDirectory, this.textFile.Text);
			if (File.Exists(file))
			{
				if (Path.GetExtension(file).ToLowerInvariant() == ".tga")
					this.picturePreview.Image = (Image)Extensions.LoadTarga(file);
				else
					this.picturePreview.Image = Image.FromFile(file);
			}
			else
				this.picturePreview.Image = null;
		}

		private void butFileChange_Click(object sender, EventArgs e)
		{
			using (var dialog = new OpenFileDialog())
			{
				dialog.InitialDirectory = this.RootDirectory;
				dialog.Multiselect = false;
				dialog.RestoreDirectory = true;
				dialog.FileName = this.textFile.Text;
				if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
				{
					var dir = Path.GetDirectoryName(dialog.FileName);
					if (dir != this.RootDirectory)
					{
						if (MessageBox.Show("File doesn't exists in main texture root directory. Copy it?", "Wrong Directory", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == System.Windows.Forms.DialogResult.Yes)
						{
							string path = Path.Combine(this.RootDirectory, dialog.SafeFileName);
							File.Copy(dialog.FileName, path);
						}
						else
							return;
					}

					this.textFile.Text = dialog.SafeFileName;
				}
			}
		}
	}
}
