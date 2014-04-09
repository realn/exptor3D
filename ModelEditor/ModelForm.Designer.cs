namespace ModelEditor
{
	partial class ModelForm
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
			this.splitPanel = new System.Windows.Forms.SplitContainer();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.button1 = new System.Windows.Forms.Button();
			this.treeModelContent = new System.Windows.Forms.TreeView();
			((System.ComponentModel.ISupportInitialize)(this.splitPanel)).BeginInit();
			this.splitPanel.Panel1.SuspendLayout();
			this.splitPanel.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// splitPanel
			// 
			this.splitPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitPanel.Location = new System.Drawing.Point(0, 0);
			this.splitPanel.Name = "splitPanel";
			// 
			// splitPanel.Panel1
			// 
			this.splitPanel.Panel1.Controls.Add(this.treeModelContent);
			this.splitPanel.Panel1.Controls.Add(this.groupBox1);
			this.splitPanel.Size = new System.Drawing.Size(549, 412);
			this.splitPanel.SplitterDistance = 176;
			this.splitPanel.TabIndex = 0;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.button1);
			this.groupBox1.Location = new System.Drawing.Point(12, 351);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(161, 49);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "groupBox1";
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(6, 19);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 0;
			this.button1.Text = "button1";
			this.button1.UseVisualStyleBackColor = true;
			// 
			// treeModelContent
			// 
			this.treeModelContent.CheckBoxes = true;
			this.treeModelContent.Location = new System.Drawing.Point(12, 12);
			this.treeModelContent.Name = "treeModelContent";
			this.treeModelContent.Size = new System.Drawing.Size(161, 333);
			this.treeModelContent.TabIndex = 1;
			// 
			// ModelForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(549, 412);
			this.Controls.Add(this.splitPanel);
			this.Name = "ModelForm";
			this.Text = "ModelForm";
			this.splitPanel.Panel1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitPanel)).EndInit();
			this.splitPanel.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer splitPanel;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.TreeView treeModelContent;
		private System.Windows.Forms.Button button1;
	}
}