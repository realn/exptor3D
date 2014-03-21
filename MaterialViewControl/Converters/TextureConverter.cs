using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace MaterialViewControl.Converters
{
	class TextureConverter : ExpandableObjectConverter
	{
		public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
		{
			if (destinationType == typeof(string))
				return true;
			return base.CanConvertTo(context, destinationType);
		}

		public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
		{
			if (destinationType == typeof(string))
			{
				if (value is Texture)
				{
					var texture = value as Texture;
					return string.Format("ID: {0}, File: {1}", texture.ID, texture.FileName);
				}
				else
					return "None";
			}
			return base.ConvertTo(context, culture, value, destinationType);
		}
	}
}
