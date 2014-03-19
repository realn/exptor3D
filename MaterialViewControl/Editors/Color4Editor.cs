using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Design;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MaterialViewControl.Editors
{
	class Color4Editor : ColorEditor
	{
		public override object EditValue(System.ComponentModel.ITypeDescriptorContext context, IServiceProvider provider, object value)
		{
			if (value is OpenTK.Graphics.Color4)
			{
				var col = (OpenTK.Graphics.Color4)value;
				var obj = base.EditValue(context, provider, Color.FromArgb(col.ToArgb()));
				if(obj is Color)
				{
					var rcol = (Color)obj;
					return new OpenTK.Graphics.Color4(rcol.R, rcol.G, rcol.B, rcol.A);
				}
			}
			return base.EditValue(context, provider, value);
		}

		public override void PaintValue(PaintValueEventArgs e)
		{
			if (e.Value is OpenTK.Graphics.Color4)
			{
				var col = (OpenTK.Graphics.Color4)e.Value;
				base.PaintValue(new PaintValueEventArgs(e.Context, Color.FromArgb(col.ToArgb()), e.Graphics, e.Bounds));
			}
			base.PaintValue(e);
		}
	}
}
