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
	[Category(Constants.CATEGORY_LIGHTING)]
	public enum LightingLevel
	{
		Emmission,
		Diffuse,
		Ambient,
		Specular,
	}

	[Serializable]
	[Category(Constants.CATEGORY_LIGHTING)]
	public class MaterialLevel
	{
		private Texture texture;
		private Color4 color;

		[Browsable(false)]
		public Material ParentMaterial { get; private set; }

		[ReadOnly(true)]
		public LightingLevel Level { get; set; }

		[TypeConverter(typeof(Converters.Color4Converter))]
		[Editor(typeof(Editors.Color4Editor), typeof(UITypeEditor))]
		public Color4 Color { 
			get { return this.color; } 
			set{
				this.color = value;
				if (this.MaterialChanged != null)
					this.MaterialChanged(this, new EventArgs());
			}
		}

		[ReadOnly(true)]
		public bool HasTexture { get { return this.Texture != null; } }

		[TypeConverter(typeof(Converters.TextureConverter))]
		[Editor(typeof(Editors.TextureUIEditor), typeof(UITypeEditor))]
		public Texture Texture
		{
			get { return this.texture; }
			set
			{
				this.texture = value;
				if (this.MaterialChanged != null)
					this.MaterialChanged(this, new EventArgs());
			}
		}

		public event EventHandler MaterialChanged;

		public MaterialLevel(Material parent, LightingLevel level)
		{
			this.ParentMaterial = parent;
			this.Level = level;
			this.Color = new Color4(0.5f, 0.5f, 0.5f, 1.0f);
			this.Texture = null;
		}
	}

	[Serializable]
	public class Material
	{
		[Browsable(false)]
		public MaterialList ParentList { get; private set; }

		[ReadOnly(true)]
		public string ID { get; set; }

		[TypeConverter(typeof(Converters.MaterialLevelConverter))]
		public MaterialLevel Diffuse { get; private set; }

		public event EventHandler MaterialChanged;

		public Material(string id, MaterialList materialList)
		{
			this.ID = id;
			this.ParentList = materialList;
			this.Diffuse = new MaterialLevel(this, LightingLevel.Diffuse);
			this.Diffuse.MaterialChanged += Diffuse_MaterialChanged;
		}

		private void Diffuse_MaterialChanged(object sender, EventArgs e)
		{
			if (this.MaterialChanged != null)
				this.MaterialChanged(this, new EventArgs());
		}
	}

	[Serializable]
	public class MaterialList : ICollection<Material>
	{
		public TextureList TextureList { get; private set; }
		List<Material> List;

		public event EventHandler MaterialChanged;

		public MaterialList()
		{
			this.TextureList = new TextureList(string.Empty);
			this.List = new List<Material>();
		}

		public Material this[string index]
		{
			get { return List.FirstOrDefault((mat) => { return mat.ID == index; }); }
		}

		public void Add(Material item)
		{
			List.Add(item);
			item.MaterialChanged += item_MaterialChanged;
		}

		public void Clear()
		{
			foreach (var mat in this.List)
				mat.MaterialChanged -= item_MaterialChanged;
			List.Clear();
		}

		public bool Contains(Material item)
		{
			return this.List.Contains(item);
		}

		public bool ContainsID(string id)
		{
			return this.FirstOrDefault((mat) => { return mat.ID == id; }) != null;
		}

		public void CopyTo(Material[] array, int arrayIndex)
		{
			this.CopyTo(array, arrayIndex);
		}

		public int Count
		{
			get { return this.Count; }
		}

		public bool IsReadOnly
		{
			get { return false; }
		}

		public bool Remove(Material item)
		{
			if (List.Remove(item))
			{
				item.MaterialChanged -= item_MaterialChanged;
				return true;
			}
			return false;
		}

		public IEnumerator<Material> GetEnumerator()
		{
			return List.GetEnumerator();
		}

		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
		{
			return List.GetEnumerator();
		}

		private void item_MaterialChanged(object sender, EventArgs e)
		{
			if (this.MaterialChanged != null)
				this.MaterialChanged(this, new EventArgs());
		}
	}
}
