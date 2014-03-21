using MaterialViewControl;
using System;
using System.Collections.Generic;
using System.Drawing.Design;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace MaterialViewControl.Editors
{
	class TextureUIEditor : UITypeEditor
	{
		public override UITypeEditorEditStyle GetEditStyle(System.ComponentModel.ITypeDescriptorContext context)
		{
			return UITypeEditorEditStyle.DropDown;
		}

		public override bool IsDropDownResizable
		{
			get
			{
				return true;
			}
		}

		public override object EditValue(System.ComponentModel.ITypeDescriptorContext context, IServiceProvider provider, object value)
		{
			var matLevel = context.Instance as MaterialLevel;
			var texture = value as Texture;
			var service = provider.GetService(typeof(IWindowsFormsEditorService)) as IWindowsFormsEditorService;

			if (matLevel != null && service != null)
			{
				var material = matLevel.ParentMaterial;
				var matList = material.ParentList;

				using (var control = new TextureManagerDialogView())
				{
					control.EditorService = service;

					control.TextureDirectory = matList.TextureList.Directory;
					control.TextureDB = matList.TextureList.TextureDB;

					if (texture != null)
						control.TextureSelected = texture.ID;

					service.DropDownControl(control);

					matList.TextureList.Directory = control.TextureDirectory;
					matList.TextureList.TextureDB = control.TextureDB;

					switch (control.DialogSelectionResult)
					{
						case TextureManagerDialogView.SelectionResult.Selected:
							return matList.TextureList.Get(control.TextureSelected);

						case TextureManagerDialogView.SelectionResult.None:
							return null;

						default:
						case TextureManagerDialogView.SelectionResult.Canceled:
							return value;
					}
				}
			}

			return value;
		}
	}
}
