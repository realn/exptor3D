namespace ModelEditor
{
	partial class MainView
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainView));
			this.materialListControl1 = new MaterialViewControl.MaterialListControl();
			this.SuspendLayout();
			// 
			// materialListControl1
			// 
			this.materialListControl1.Location = new System.Drawing.Point(37, 40);
			this.materialListControl1.Margin = new System.Windows.Forms.Padding(6);
			this.materialListControl1.MaterialList = ((System.Collections.Generic.List<MaterialViewControl.Material>)(resources.GetObject("materialListControl1.MaterialList")));
			this.materialListControl1.Name = "materialListControl1";
			this.materialListControl1.Size = new System.Drawing.Size(504, 316);
			this.materialListControl1.TabIndex = 0;
			// 
			// MainView
			// 
			this.ClientSize = new System.Drawing.Size(575, 400);
			this.Controls.Add(this.materialListControl1);
			this.Name = "MainView";
			this.ResumeLayout(false);

		}

		#endregion

		private MaterialViewControl.MaterialListControl materialListControl1;



	}
}