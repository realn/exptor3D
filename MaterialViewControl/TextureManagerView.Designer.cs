namespace MaterialViewControl
{
	partial class TextureManagerView
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
			this.groupRootFolder = new System.Windows.Forms.GroupBox();
			this.butBrowseRoot = new System.Windows.Forms.Button();
			this.splitTextureProps = new System.Windows.Forms.SplitContainer();
			this.pictureTextureImage = new System.Windows.Forms.PictureBox();
			this.textTexRootDir = new System.Windows.Forms.TextBox();
			this.butAction = new System.Windows.Forms.Button();
			this.listTexList = new System.Windows.Forms.ListView();
			this.colTexID = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.colTexFile = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.comboAction = new System.Windows.Forms.ComboBox();
			this.groupRootFolder.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitTextureProps)).BeginInit();
			this.splitTextureProps.Panel1.SuspendLayout();
			this.splitTextureProps.Panel2.SuspendLayout();
			this.splitTextureProps.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.pictureTextureImage)).BeginInit();
			this.SuspendLayout();
			// 
			// groupRootFolder
			// 
			this.groupRootFolder.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupRootFolder.Controls.Add(this.textTexRootDir);
			this.groupRootFolder.Controls.Add(this.butBrowseRoot);
			this.groupRootFolder.Location = new System.Drawing.Point(3, 183);
			this.groupRootFolder.Name = "groupRootFolder";
			this.groupRootFolder.Size = new System.Drawing.Size(428, 45);
			this.groupRootFolder.TabIndex = 0;
			this.groupRootFolder.TabStop = false;
			this.groupRootFolder.Text = "Root Folder";
			// 
			// butBrowseRoot
			// 
			this.butBrowseRoot.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.butBrowseRoot.Location = new System.Drawing.Point(347, 15);
			this.butBrowseRoot.Name = "butBrowseRoot";
			this.butBrowseRoot.Size = new System.Drawing.Size(75, 23);
			this.butBrowseRoot.TabIndex = 0;
			this.butBrowseRoot.Text = "Change";
			this.butBrowseRoot.UseVisualStyleBackColor = true;
			this.butBrowseRoot.Click += new System.EventHandler(this.butBrowseRoot_Click);
			// 
			// splitTextureProps
			// 
			this.splitTextureProps.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.splitTextureProps.Location = new System.Drawing.Point(3, 3);
			this.splitTextureProps.Name = "splitTextureProps";
			// 
			// splitTextureProps.Panel1
			// 
			this.splitTextureProps.Panel1.Controls.Add(this.pictureTextureImage);
			// 
			// splitTextureProps.Panel2
			// 
			this.splitTextureProps.Panel2.Controls.Add(this.comboAction);
			this.splitTextureProps.Panel2.Controls.Add(this.listTexList);
			this.splitTextureProps.Panel2.Controls.Add(this.butAction);
			this.splitTextureProps.Size = new System.Drawing.Size(428, 174);
			this.splitTextureProps.SplitterDistance = 194;
			this.splitTextureProps.TabIndex = 1;
			// 
			// pictureTextureImage
			// 
			this.pictureTextureImage.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.pictureTextureImage.Location = new System.Drawing.Point(3, 3);
			this.pictureTextureImage.Name = "pictureTextureImage";
			this.pictureTextureImage.Size = new System.Drawing.Size(188, 168);
			this.pictureTextureImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.pictureTextureImage.TabIndex = 0;
			this.pictureTextureImage.TabStop = false;
			// 
			// textTexRootDir
			// 
			this.textTexRootDir.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textTexRootDir.Location = new System.Drawing.Point(3, 17);
			this.textTexRootDir.Name = "textTexRootDir";
			this.textTexRootDir.ReadOnly = true;
			this.textTexRootDir.Size = new System.Drawing.Size(338, 20);
			this.textTexRootDir.TabIndex = 1;
			// 
			// butAction
			// 
			this.butAction.Location = new System.Drawing.Point(149, 143);
			this.butAction.Name = "butAction";
			this.butAction.Size = new System.Drawing.Size(75, 23);
			this.butAction.TabIndex = 1;
			this.butAction.Text = "Perform";
			this.butAction.UseVisualStyleBackColor = true;
			this.butAction.Click += new System.EventHandler(this.butAction_Click);
			// 
			// listTexList
			// 
			this.listTexList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colTexID,
            this.colTexFile});
			this.listTexList.FullRowSelect = true;
			this.listTexList.GridLines = true;
			this.listTexList.HideSelection = false;
			this.listTexList.Location = new System.Drawing.Point(3, 3);
			this.listTexList.MultiSelect = false;
			this.listTexList.Name = "listTexList";
			this.listTexList.Size = new System.Drawing.Size(221, 134);
			this.listTexList.TabIndex = 3;
			this.listTexList.UseCompatibleStateImageBehavior = false;
			this.listTexList.View = System.Windows.Forms.View.Details;
			// 
			// colTexID
			// 
			this.colTexID.Text = "ID";
			this.colTexID.Width = 82;
			// 
			// colTexFile
			// 
			this.colTexFile.Text = "File";
			this.colTexFile.Width = 135;
			// 
			// comboAction
			// 
			this.comboAction.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboAction.FormattingEnabled = true;
			this.comboAction.Location = new System.Drawing.Point(3, 145);
			this.comboAction.Name = "comboAction";
			this.comboAction.Size = new System.Drawing.Size(140, 21);
			this.comboAction.TabIndex = 4;
			// 
			// TextureManagerView
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.splitTextureProps);
			this.Controls.Add(this.groupRootFolder);
			this.Name = "TextureManagerView";
			this.Size = new System.Drawing.Size(434, 231);
			this.groupRootFolder.ResumeLayout(false);
			this.groupRootFolder.PerformLayout();
			this.splitTextureProps.Panel1.ResumeLayout(false);
			this.splitTextureProps.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitTextureProps)).EndInit();
			this.splitTextureProps.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.pictureTextureImage)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupRootFolder;
		private System.Windows.Forms.TextBox textTexRootDir;
		private System.Windows.Forms.Button butBrowseRoot;
		private System.Windows.Forms.SplitContainer splitTextureProps;
		private System.Windows.Forms.PictureBox pictureTextureImage;
		private System.Windows.Forms.ComboBox comboAction;
		private System.Windows.Forms.ListView listTexList;
		private System.Windows.Forms.ColumnHeader colTexID;
		private System.Windows.Forms.ColumnHeader colTexFile;
		private System.Windows.Forms.Button butAction;
	}
}
