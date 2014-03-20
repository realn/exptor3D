using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MaterialViewControl
{
	internal static class Extensions
	{
		public static void SetEnum<_T>(this ComboBox combo) where _T : struct
		{
			if (!typeof(_T).IsEnum)
				return;

			combo.DataSource = Enum.GetValues(typeof(_T));
		}

		public static _T GetSelectedEnum<_T>(this ComboBox combo) where _T : struct
		{
			_T result = new _T();
			if (Enum.TryParse<_T>(combo.SelectedText, true, out result))
				return result;
			return new _T();
		}

		public static Bitmap LoadTarga(string filename)
		{
			byte[] TGAheader = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			using (var file = System.IO.File.OpenRead(filename))
			using (var reader = new System.IO.BinaryReader(file))
			{
				byte[] headerMain = reader.ReadBytes(TGAheader.Length);
				if (!headerMain.SequenceEqual(TGAheader))
					return null;

				byte[] header = reader.ReadBytes(6);

				var width = (int)(header[1]) * 256 + (int)(header[0]);		// Opracuj szerokość obrazka
				var height = (int)(header[3]) * 256 + (int)(header[2]);		// i jego wysokosc

				if (width <= 0 ||			// Jeżeli szerokość
					height <= 0 ||			// lub wysokośc jest mniejsza lub równa zeru
					(header[4] != 24 && header[4] != 32))		// Lub to nie jest ani obraz 24 Bity, ani 32 Bity
				{
					return null;
				}
				var bpp = header[4];
				var bytesPerPixel = bpp / 8;
				var imageSize = width * height * bytesPerPixel;

				var data = reader.ReadBytes(imageSize);

				System.Drawing.Imaging.PixelFormat graphicFormat;
				switch (bpp)
				{
					case 24:
						graphicFormat = System.Drawing.Imaging.PixelFormat.Format24bppRgb;
						break;

					default:
					case 32:
						graphicFormat = System.Drawing.Imaging.PixelFormat.Format32bppArgb;
						break;
				}

				var graphic = new Bitmap(width, height, graphicFormat);

				var dataLock = graphic.LockBits(new Rectangle(0, 0, width, height), System.Drawing.Imaging.ImageLockMode.WriteOnly, graphicFormat);
				Marshal.Copy(data, 0, dataLock.Scan0, data.Length);
				graphic.UnlockBits(dataLock);

				return graphic;
			}
		}
	}
}
