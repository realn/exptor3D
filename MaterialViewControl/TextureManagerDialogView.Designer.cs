namespace MaterialViewControl
{
	partial class TextureManagerDialogView
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TextureManagerDialogView));
			this.textureView = new MaterialViewControl.TextureManagerView();
			this.butSelect = new System.Windows.Forms.Button();
			this.butNone = new System.Windows.Forms.Button();
			this.butCancel = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// textureView
			// 
			this.textureView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textureView.Location = new System.Drawing.Point(3, 3);
			this.textureView.Name = "textureView";
			this.textureView.SelectedTextureID = null;
			this.textureView.Size = new System.Drawing.Size(439, 236);
			this.textureView.TabIndex = 0;
			this.textureView.TextureDB = ((System.Collections.Generic.Dictionary<string, string>)(resources.GetObject("textureView.TextureDB")));
			this.textureView.TextureDirectory = "";
			// 
			// butSelect
			// 
			this.butSelect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.butSelect.Location = new System.Drawing.Point(205, 245);
			this.butSelect.Name = "butSelect";
			this.butSelect.Size = new System.Drawing.Size(75, 23);
			this.butSelect.TabIndex = 1;
			this.butSelect.Text = "Select";
			this.butSelect.UseVisualStyleBackColor = true;
			this.butSelect.Click += new System.EventHandler(this.butSelect_Click);
			// 
			// butNone
			// 
			this.butNone.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.butNone.Location = new System.Drawing.Point(286, 245);
			this.butNone.Name = "butNone";
			this.butNone.Size = new System.Drawing.Size(75, 23);
			this.butNone.TabIndex = 2;
			this.butNone.Text = "None";
			this.butNone.UseVisualStyleBackColor = true;
			this.butNone.Click += new System.EventHandler(this.butNone_Click);
			// 
			// butCancel
			// 
			this.butCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.butCancel.Location = new System.Drawing.Point(367, 245);
			this.butCancel.Name = "butCancel";
			this.butCancel.Size = new System.Drawing.Size(75, 23);
			this.butCancel.TabIndex = 3;
			this.butCancel.Text = "Cancel";
			this.butCancel.UseVisualStyleBackColor = true;
			this.butCancel.Click += new System.EventHandler(this.butCancel_Click);
			// 
			// TextureManagerDialogView
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.butCancel);
			this.Controls.Add(this.butNone);
			this.Controls.Add(this.butSelect);
			this.Controls.Add(this.textureView);
			this.Name = "TextureManagerDialogView";
			this.Size = new System.Drawing.Size(445, 271);
			this.ResumeLayout(false);

		}

		#endregion

		private MaterialViewControl.TextureManagerView textureView;
		private System.Windows.Forms.Button butSelect;
		private System.Windows.Forms.Button butNone;
		private System.Windows.Forms.Button butCancel;
	}
}
