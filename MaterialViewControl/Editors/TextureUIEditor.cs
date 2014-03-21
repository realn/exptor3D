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
			var mat = context.Instance as MaterialLevel;
			var texture = value as Texture;
			var service = provider.GetService(typeof(IWindowsFormsEditorService)) as IWindowsFormsEditorService;

			if (mat != null && service != null)
			{
				using (var control = new TextureManagerDialogView())
				{
					control.EditorService = service;

					service.DropDownControl(control);
				}
			}

			return value;
		}
	}
}
