﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace MaterialViewControl
{
	public partial class TextureManagerDialogView : UserControl
	{
		public enum SelectionResult
		{
			Selected,
			None,
			Canceled,
		}
		public string TextureDirectory
		{
			get { return this.textureView.TextureDirectory; }
			set { this.textureView.TextureDirectory = value; }
		}
		public Dictionary<string, string> TextureDB
		{
			get { return this.textureView.TextureDB; }
			set { this.textureView.TextureDB = value; }
		}
		public string TextureSelected
		{
			get { return this.textureView.SelectedTextureID; }
			set { this.textureView.SelectedTextureID = value; }
		}
		public SelectionResult DialogSelectionResult { get; set; }

		public IWindowsFormsEditorService EditorService { get; set; }

		public TextureManagerDialogView()
		{
			InitializeComponent();
		}

		private void butSelect_Click(object sender, EventArgs e)
		{
			this.DialogSelectionResult = string.IsNullOrEmpty(this.TextureSelected) ? SelectionResult.None : SelectionResult.None;
			if (this.EditorService != null)
				this.EditorService.CloseDropDown();
		}

		private void butNone_Click(object sender, EventArgs e)
		{
			this.DialogSelectionResult = SelectionResult.None;
			if (this.EditorService != null)
				this.EditorService.CloseDropDown();
		}

		private void butCancel_Click(object sender, EventArgs e)
		{
			this.DialogSelectionResult = SelectionResult.Canceled;
			if (this.EditorService != null)
				this.EditorService.CloseDropDown();
		}


	}
}