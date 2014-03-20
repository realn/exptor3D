namespace MaterialViewControl
{
	partial class TextureEditDialog
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

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.butOK = new System.Windows.Forms.Button();
			this.butCancel = new System.Windows.Forms.Button();
			this.groupPreview = new System.Windows.Forms.GroupBox();
			this.picturePreview = new System.Windows.Forms.PictureBox();
			this.groupID = new System.Windows.Forms.GroupBox();
			this.textID = new System.Windows.Forms.TextBox();
			this.groupFile = new System.Windows.Forms.GroupBox();
			this.butFileChange = new System.Windows.Forms.Button();
			this.textFile = new System.Windows.Forms.TextBox();
			this.groupPreview.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.picturePreview)).BeginInit();
			this.groupID.SuspendLayout();
			this.groupFile.SuspendLayout();
			this.SuspendLayout();
			// 
			// butOK
			// 
			this.butOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.butOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.butOK.Location = new System.Drawing.Point(278, 144);
			this.butOK.Name = "butOK";
			this.butOK.Size = new System.Drawing.Size(75, 23);
			this.butOK.TabIndex = 0;
			this.butOK.Text = "OK";
			this.butOK.UseVisualStyleBackColor = true;
			// 
			// butCancel
			// 
			this.butCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.butCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.butCancel.Location = new System.Drawing.Point(359, 144);
			this.butCancel.Name = "butCancel";
			this.butCancel.Size = new System.Drawing.Size(75, 23);
			this.butCancel.TabIndex = 1;
			this.butCancel.Text = "Cancel";
			this.butCancel.UseVisualStyleBackColor = true;
			// 
			// groupPreview
			// 
			this.groupPreview.Controls.Add(this.picturePreview);
			this.groupPreview.Location = new System.Drawing.Point(12, 12);
			this.groupPreview.Name = "groupPreview";
			this.groupPreview.Size = new System.Drawing.Size(133, 145);
			this.groupPreview.TabIndex = 2;
			this.groupPreview.TabStop = false;
			this.groupPreview.Text = "Preview";
			// 
			// picturePreview
			// 
			this.picturePreview.Location = new System.Drawing.Point(6, 19);
			this.picturePreview.Name = "picturePreview";
			this.picturePreview.Size = new System.Drawing.Size(121, 121);
			this.picturePreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.picturePreview.TabIndex = 0;
			this.picturePreview.TabStop = false;
			// 
			// groupID
			// 
			this.groupID.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupID.Controls.Add(this.textID);
			this.groupID.Location = new System.Drawing.Point(153, 12);
			this.groupID.Name = "groupID";
			this.groupID.Size = new System.Drawing.Size(281, 48);
			this.groupID.TabIndex = 3;
			this.groupID.TabStop = false;
			this.groupID.Text = "ID";
			// 
			// textID
			// 
			this.textID.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textID.Location = new System.Drawing.Point(6, 19);
			this.textID.Name = "textID";
			this.textID.Size = new System.Drawing.Size(269, 20);
			this.textID.TabIndex = 0;
			// 
			// groupFile
			// 
			this.groupFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupFile.Controls.Add(this.textFile);
			this.groupFile.Controls.Add(this.butFileChange);
			this.groupFile.Location = new System.Drawing.Point(151, 66);
			this.groupFile.Name = "groupFile";
			this.groupFile.Size = new System.Drawing.Size(283, 54);
			this.groupFile.TabIndex = 4;
			this.groupFile.TabStop = false;
			this.groupFile.Text = "File";
			// 
			// butFileChange
			// 
			this.butFileChange.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.butFileChange.Location = new System.Drawing.Point(202, 19);
			this.butFileChange.Name = "butFileChange";
			this.butFileChange.Size = new System.Drawing.Size(75, 23);
			this.butFileChange.TabIndex = 0;
			this.butFileChange.Text = "Change";
			this.butFileChange.UseVisualStyleBackColor = true;
			this.butFileChange.Click += new System.EventHandler(this.butFileChange_Click);
			// 
			// textFile
			// 
			this.textFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textFile.Location = new System.Drawing.Point(6, 21);
			this.textFile.Name = "textFile";
			this.textFile.Size = new System.Drawing.Size(190, 20);
			this.textFile.TabIndex = 1;
			this.textFile.TextChanged += new System.EventHandler(this.textFile_TextChanged);
			// 
			// TextureEditDialog
			// 
			this.AcceptButton = this.butOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.butCancel;
			this.ClientSize = new System.Drawing.Size(443, 179);
			this.ControlBox = false;
			this.Controls.Add(this.groupFile);
			this.Controls.Add(this.groupID);
			this.Controls.Add(this.groupPreview);
			this.Controls.Add(this.butCancel);
			this.Controls.Add(this.butOK);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "TextureEditDialog";
			this.Text = "TextureEdit";
			this.groupPreview.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.picturePreview)).EndInit();
			this.groupID.ResumeLayout(false);
			this.groupID.PerformLayout();
			this.groupFile.ResumeLayout(false);
			this.groupFile.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button butOK;
		private System.Windows.Forms.Button butCancel;
		private System.Windows.Forms.GroupBox groupPreview;
		private System.Windows.Forms.PictureBox picturePreview;
		private System.Windows.Forms.GroupBox groupID;
		private System.Windows.Forms.TextBox textID;
		private System.Windows.Forms.GroupBox groupFile;
		private System.Windows.Forms.TextBox textFile;
		private System.Windows.Forms.Button butFileChange;
	}
}