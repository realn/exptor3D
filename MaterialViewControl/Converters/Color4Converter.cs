using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Drawing;

namespace MaterialViewControl.Converters
{
	class Color4Converter : ColorConverter
	{
		public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
		{
			var col = (OpenTK.Graphics.Color4)value;
			if (destinationType == typeof(Color))
				return base.ConvertTo(context, culture, Color.FromArgb(col.ToArgb()), destinationType);
			if (destinationType == typeof(string))
				return string.Format("({0};{1};{2};{3})", col.R, col.G, col.B, col.A);
			return base.ConvertTo(context, culture, value, destinationType);
		}

		public override object ConvertFrom(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value)
		{
			if (value is Color)
			{
				var col = (Color)value;

				return new OpenTK.Graphics.Color4(col.R, col.G, col.B, col.A);
			}
			return base.ConvertFrom(context, culture, value);
		}

		public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
		{
			return destinationType == typeof(string) || destinationType == typeof(Color);
		}

		public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
		{
			return sourceType == typeof(Color);
		}
	}
}
