using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MaterialViewControl
{
	namespace Converters
	{
		class MaterialLevelConverter : ExpandableObjectConverter
		{
			public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
			{
				if (destinationType == typeof(string))
					return true;
				return false;
			}

			public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
			{
				if (value is MaterialLevel && destinationType == typeof(string))
				{
					var mat = value as MaterialLevel;

					return "Level: " + mat.Level.ToString() + ", Texture: " + mat.HasTexture.ToString();
				}
				return base.ConvertTo(context, culture, value, destinationType);
			}
		}
	}
}
