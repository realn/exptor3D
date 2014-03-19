namespace MaterialViewControl
{
    partial class MaterialListControl
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
			this.groupMaterialList = new System.Windows.Forms.GroupBox();
			this.ButMatDel = new System.Windows.Forms.Button();
			this.butMatEdit = new System.Windows.Forms.Button();
			this.butMatAdd = new System.Windows.Forms.Button();
			this.propMaterial = new System.Windows.Forms.PropertyGrid();
			this.comboMaterialList = new System.Windows.Forms.ComboBox();
			this.viewMaterial = new MaterialViewControl.MaterialView();
			this.a = new System.Windows.Forms.SplitContainer();
			this.groupMaterialList.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.a)).BeginInit();
			this.a.Panel1.SuspendLayout();
			this.a.Panel2.SuspendLayout();
			this.a.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupMaterialList
			// 
			this.groupMaterialList.Controls.Add(this.ButMatDel);
			this.groupMaterialList.Controls.Add(this.butMatEdit);
			this.groupMaterialList.Controls.Add(this.butMatAdd);
			this.groupMaterialList.Controls.Add(this.propMaterial);
			this.groupMaterialList.Controls.Add(this.comboMaterialList);
			this.groupMaterialList.Dock = System.Windows.Forms.DockStyle.Fill;
			this.groupMaterialList.Location = new System.Drawing.Point(0, 0);
			this.groupMaterialList.MinimumSize = new System.Drawing.Size(253, 0);
			this.groupMaterialList.Name = "groupMaterialList";
			this.groupMaterialList.Size = new System.Drawing.Size(257, 316);
			this.groupMaterialList.TabIndex = 0;
			this.groupMaterialList.TabStop = false;
			this.groupMaterialList.Text = "MaterialList";
			// 
			// ButMatDel
			// 
			this.ButMatDel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ButMatDel.Location = new System.Drawing.Point(176, 46);
			this.ButMatDel.Name = "ButMatDel";
			this.ButMatDel.Size = new System.Drawing.Size(75, 23);
			this.ButMatDel.TabIndex = 4;
			this.ButMatDel.Text = "button3";
			this.ButMatDel.UseVisualStyleBackColor = true;
			// 
			// butMatEdit
			// 
			this.butMatEdit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.butMatEdit.Location = new System.Drawing.Point(95, 46);
			this.butMatEdit.Name = "butMatEdit";
			this.butMatEdit.Size = new System.Drawing.Size(75, 23);
			this.butMatEdit.TabIndex = 3;
			this.butMatEdit.Text = "button2";
			this.butMatEdit.UseVisualStyleBackColor = true;
			// 
			// butMatAdd
			// 
			this.butMatAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.butMatAdd.Location = new System.Drawing.Point(11, 46);
			this.butMatAdd.Name = "butMatAdd";
			this.butMatAdd.Size = new System.Drawing.Size(75, 23);
			this.butMatAdd.TabIndex = 2;
			this.butMatAdd.Text = "Add";
			this.butMatAdd.UseVisualStyleBackColor = true;
			this.butMatAdd.Click += new System.EventHandler(this.butMatAdd_Click);
			// 
			// propMaterial
			// 
			this.propMaterial.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.propMaterial.Location = new System.Drawing.Point(6, 75);
			this.propMaterial.Name = "propMaterial";
			this.propMaterial.Size = new System.Drawing.Size(245, 235);
			this.propMaterial.TabIndex = 1;
			// 
			// comboMaterialList
			// 
			this.comboMaterialList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.comboMaterialList.FormattingEnabled = true;
			this.comboMaterialList.Location = new System.Drawing.Point(6, 19);
			this.comboMaterialList.Name = "comboMaterialList";
			this.comboMaterialList.Size = new System.Drawing.Size(245, 21);
			this.comboMaterialList.TabIndex = 0;
			// 
			// viewMaterial
			// 
			this.viewMaterial.Dock = System.Windows.Forms.DockStyle.Fill;
			this.viewMaterial.Location = new System.Drawing.Point(0, 0);
			this.viewMaterial.Material = null;
			this.viewMaterial.Name = "viewMaterial";
			this.viewMaterial.Size = new System.Drawing.Size(243, 316);
			this.viewMaterial.TabIndex = 1;
			// 
			// a
			// 
			this.a.Dock = System.Windows.Forms.DockStyle.Fill;
			this.a.Location = new System.Drawing.Point(0, 0);
			this.a.Name = "a";
			// 
			// a.Panel1
			// 
			this.a.Panel1.Controls.Add(this.viewMaterial);
			// 
			// a.Panel2
			// 
			this.a.Panel2.Controls.Add(this.groupMaterialList);
			this.a.Size = new System.Drawing.Size(504, 316);
			this.a.SplitterDistance = 243;
			this.a.TabIndex = 2;
			// 
			// MaterialListControl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.a);
			this.Margin = new System.Windows.Forms.Padding(6);
			this.Name = "MaterialListControl";
			this.Size = new System.Drawing.Size(504, 316);
			this.groupMaterialList.ResumeLayout(false);
			this.a.Panel1.ResumeLayout(false);
			this.a.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.a)).EndInit();
			this.a.ResumeLayout(false);
			this.ResumeLayout(false);

        }

        #endregion

				private System.Windows.Forms.GroupBox groupMaterialList;
				private System.Windows.Forms.Button ButMatDel;
				private System.Windows.Forms.Button butMatEdit;
				private System.Windows.Forms.Button butMatAdd;
				private System.Windows.Forms.PropertyGrid propMaterial;
				private System.Windows.Forms.ComboBox comboMaterialList;
				private MaterialView viewMaterial;
				private System.Windows.Forms.SplitContainer a;
    }
}
