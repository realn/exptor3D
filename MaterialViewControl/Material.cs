using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using System.Runtime.InteropServices;
using System.ComponentModel;
using System.Drawing.Design;

namespace MaterialViewControl
{
	[Serializable]
	public enum LightingLevel
	{
		Emmission,
		Diffuse,
		Ambient,
		Specular,
	}

	[Serializable]
	public class MaterialLevel
	{
		public LightingLevel Level { get; set; }

		[TypeConverter(typeof(Converters.Color4Converter))]
		[Editor(typeof(Editors.Color4Editor), typeof(UITypeEditor))]
		public Color4 Color { get; set; }

		public bool HasTexture { get { return this.Texture != null; } }

		[TypeConverter(typeof(Converters.TextureConverter))]
		[Editor(typeof(Editors.TextureUIEditor), typeof(UITypeEditor))]
		public Texture Texture { get; set; }

		public MaterialLevel(LightingLevel level)
		{
			this.Level = level;
			this.Color = new Color4(0.5f, 0.5f, 0.5f, 1.0f);
			this.Texture = null;
		}
	}

	[Serializable]
	public class Material
	{
		public string Name { get; set; }

		[TypeConverter(typeof(Converters.MaterialLevelConverter))]
		public MaterialLevel Diffuse { get; set; }

		public Material(string name)
		{
			this.Name = name;
			this.Diffuse = new MaterialLevel(LightingLevel.Diffuse);
		}
	}
}
