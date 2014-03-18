using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using System.Globalization;

namespace ModelEditor
{
	internal static class Extensions
	{
		public static PrimitiveType ToPrimitiveType(this string text)
		{
			if (text == "GL_TRIANGLES")
				return PrimitiveType.Triangles;
			if (text == "GL_LINES")
				return PrimitiveType.Lines;
			if (text == "GL_POINTS")
				return PrimitiveType.Points;

			return PrimitiveType.Triangles;
		}

		public static Vector2 ToVector2(this IList<string> list, int offset)
		{
			if (offset + 1 >= list.Count)
				return new Vector2();

			for (int i = offset; i < offset + 2; i++)
			{
				int index = list[i].IndexOf("f");
				if (index < 0)
					continue;

				list[i] = list[i].Remove(index);
			}

			var x = float.Parse(list[offset + 0].Trim(), CultureInfo.InvariantCulture);
			var y = float.Parse(list[offset + 1].Trim(), CultureInfo.InvariantCulture);

			return new Vector2(x, y);
		}

		public static Vector2 ToVector2(this IList<string> list)
		{
			return ToVector2(list, 0);
		}

		public static Vector3 ToVector3(this IList<string> list, int offset)
		{
			if (offset + 2 >= list.Count)
				return new Vector3();

			for (int i = offset; i < offset + 3; i++)
			{
				int index = list[i].IndexOf("f");
				if (index < 0)
					continue;

				list[i] = list[i].Remove(index);
			}

			var x = float.Parse(list[offset + 0].Trim(), CultureInfo.InvariantCulture);
			var y = float.Parse(list[offset + 1].Trim(), CultureInfo.InvariantCulture);
			var z = float.Parse(list[offset + 2].Trim(), CultureInfo.InvariantCulture);

			return new Vector3(x, y, z);
		}

		public static Vector3 ToVector3(this IList<string> list)
		{
			return ToVector3(list, 0);
		}

		public static Color4 ToColor4(this IList<string> list, int offset)
		{
			if (offset + 3 >= list.Count)
				return new Color4();

			for (int i = offset; i < offset + 4; i++)
			{
				int index = list[i].IndexOf("f");
				if (index < 0)
					continue;

				list[i] = list[i].Remove(index);
			}

			var r = float.Parse(list[offset + 0].Trim(), CultureInfo.InvariantCulture);
			var g = float.Parse(list[offset + 1].Trim(), CultureInfo.InvariantCulture);
			var b = float.Parse(list[offset + 2].Trim(), CultureInfo.InvariantCulture);
			var a = float.Parse(list[offset + 3].Trim(), CultureInfo.InvariantCulture);

			return new Color4(r, g, b, a);
		}

		public static Color4 ToColor4(this IList<string> list)
		{
			return ToColor4(list, 0);
		}
	}
}
