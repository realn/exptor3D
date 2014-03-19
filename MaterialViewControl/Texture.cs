﻿using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using OpenTK.Graphics.OpenGL;
using OpenTK.Graphics;
using OpenTK;

namespace MaterialViewControl
{
	[Serializable]
	public class Texture : IDisposable
	{
		public string FileName { get; set; }
		public int GLID { get; set; }
		public bool Loaded { get { return GLID != 0; } }

		public Texture()
		{
			GLID = 0;
			FileName = string.Empty;
		}

		public Texture(string directory, string filename)
		{
			GLID = 0;
			Load(directory, filename);
		}

		~Texture()
		{
			Unload();
		}

		private Bitmap LoadTarga(string filename)
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

		public bool Load(string directory, string filename)
		{
			if (string.IsNullOrWhiteSpace(filename))
				return false;

			this.Unload();

			var path = System.IO.Path.Combine(directory, filename);
			using (var bitmap = LoadTarga(path))
			{
				if (bitmap == null)
					return false;

				GLID = GL.GenTexture();
				GL.Enable(EnableCap.Texture2D);
				GL.BindTexture(TextureTarget.Texture2D, this.GLID);
				GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);
				GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.LinearMipmapLinear);
				GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int)TextureWrapMode.Repeat);
				GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)TextureWrapMode.Repeat);
				GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.GenerateMipmap, 1);

				PixelFormat format = PixelFormat.Rgb;
				switch (bitmap.PixelFormat)
				{
					case System.Drawing.Imaging.PixelFormat.Format24bppRgb: format = PixelFormat.Bgr; break;
					case System.Drawing.Imaging.PixelFormat.Format32bppArgb: format = PixelFormat.Bgra; break;
					default:
						return false;
				}
				var lockData = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height), System.Drawing.Imaging.ImageLockMode.ReadOnly, bitmap.PixelFormat);

				GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba8, bitmap.Width, bitmap.Height, 0, format, PixelType.UnsignedByte, lockData.Scan0);

				bitmap.UnlockBits(lockData);

				GL.BindTexture(TextureTarget.Texture2D, 0);
				GL.Disable(EnableCap.Texture2D);
			}

			this.FileName = filename;
			return true;
		}

		public void Unload()
		{
			if (Loaded)
			{
				if (OpenTK.Graphics.GraphicsContext.CurrentContext != null)
					GL.DeleteTexture(this.GLID);
				this.GLID = 0;
			}
		}

		public void Dispose()
		{
			this.Unload();
		}
	}

	[Serializable]
	public class TextureList
	{
		public string Directory { get; private set; }
		List<Texture> List;

		public TextureList(string directory)
		{
			this.Directory = directory;
			this.List = new List<Texture>();
		}

		public Texture Get(string filename)
		{
			var tex = this.List.Find((item) => { return item.FileName == filename; });
			if (tex == null)
			{
				tex = new Texture();
				if (!tex.Load(this.Directory, filename))
					return null;
				this.List.Add(tex);
			}
			return tex;
		}

		public Texture this[string index]
		{
			get
			{
				return this.Get(index);
			}
		}
	}

}
