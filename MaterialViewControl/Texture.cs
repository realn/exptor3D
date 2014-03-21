using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using OpenTK.Graphics.OpenGL;
using OpenTK.Graphics;
using OpenTK;
using System.ComponentModel;

namespace MaterialViewControl
{
	[Serializable]
	public class Texture : IDisposable
	{
		[ReadOnly(true)]
		public string ID { get; set; }

		[ReadOnly(true)]
		public string FileName { get; set; }

		[Browsable(false)]
		public int GLID { get; private set; }

		[ReadOnly(true)]
		public bool Loaded { get { return GLID != 0; } }

		public Texture(string id)
		{
			this.ID = id;
			this.GLID = 0;
			this.FileName = string.Empty;
		}

		public Texture(string id, string directory, string filename) :
			this(id)
		{
			Load(directory, filename);
		}

		~Texture()
		{
			Unload();
		}

		public bool Load(string directory, string filename)
		{
			if (string.IsNullOrWhiteSpace(filename))
				return false;

			this.Unload();

			var path = System.IO.Path.Combine(directory, filename);
			using (var bitmap = Extensions.LoadBitmapFile(path))
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
				if (GraphicsContext.CurrentContext != null)
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
	public class TextureList : ICollection<Texture>
	{
		public string Directory { get; set; }
		public Dictionary<string, string> TextureDB { get; set; }

		List<Texture> List;

		public TextureList(string directory)
		{
			this.Directory = directory;
			this.TextureDB = new Dictionary<string, string>();
			this.List = new List<Texture>();
		}

		public Texture Get(string id)
		{
			var tex = this.List.Find((item) => { return item.ID == id; });
			if (tex == null)
			{
				if (!this.TextureDB.ContainsKey(id))
					return null;

				var filename = this.TextureDB[id];
				tex = new Texture(id);
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

		public void Add(Texture item)
		{
			this.List.Add(item);
		}

		public void Clear()
		{
			this.List.Clear();
		}

		public bool Contains(Texture item)
		{
			return this.List.Contains(item);
		}

		public void CopyTo(Texture[] array, int arrayIndex)
		{
			List.CopyTo(array, arrayIndex);
		}

		public int Count
		{
			get { return List.Count; }
		}

		public bool IsReadOnly
		{
			get { return false; }
		}

		public bool Remove(Texture item)
		{
			return this.List.Remove(item);
		}

		public IEnumerator<Texture> GetEnumerator()
		{
			return this.List.GetEnumerator();
		}

		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
		{
			return this.List.GetEnumerator();
		}
	}

}
