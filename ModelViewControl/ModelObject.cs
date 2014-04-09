using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using System.Globalization;
using MaterialViewControl;

namespace ModelViewControl
{
	public class KeyFrame
	{
		public float Time { get; set; }
		public Matrix4 Transform { get; set; }
		public List<Mesh> MeshList { get; private set; }

		public KeyFrame()
		{
			this.Time = 0.0f;
			this.MeshList = new List<Mesh>();
		}
	}

	public class ModelObject
	{
		public Model Parent { get; private set; }
		public string ID { get; set; }
		public Matrix4 Transform { get; set; }
		public List<KeyFrame> Frames { get; private set; }
		public List<Mesh> MeshList { get; private set; }
		public List<string> ParsingLog { get; private set; }

		public ModelObject(Model parent)
		{
			this.Parent = parent;

			this.MeshList = new List<Mesh>();
			this.Frames = new List<KeyFrame>();
			this.ParsingLog = new List<string>();
			this.Transform = Matrix4.Identity;
		}

		public void Clear()
		{
			this.Frames.Clear();
			this.MeshList.Clear();
			this.ParsingLog.Clear();
			this.Transform = Matrix4.Identity;
		}

		public void UpdateMeshes()
		{
			foreach (var mesh in MeshList)
			{
				mesh.UpdateBuffers(BufferUsageHint.StaticDraw);
			}
		}

		public void Render()
		{
			foreach (var mesh in MeshList)
			{
				mesh.Render();
			}
		}

		private Mesh GetMeshByMaterialID(string materialID, bool createIfNotFound)
		{
			var mesh = this.MeshList.FirstOrDefault(item => item.MaterialID == materialID);
			if (mesh == null && createIfNotFound)
			{
				mesh = new Mesh(PrimitiveType.Triangles);
				mesh.MaterialID = materialID;
			}
			return mesh;
		}

		public void ParseCommands(IEnumerable<RenderCommand> commandList, MaterialList materialList)
		{
			Clear();

			Vector3 normal = new Vector3(1.0f, 0.0f, 0.0f);
			Vector2 texCoord = new Vector2(0.0f, 0.0f);
			Color4 color = new Color4(1.0f, 1.0f, 1.0f, 1.0f);
			Mesh mesh = null;

			foreach (var cmd in commandList)
			{
				if (cmd.Name == "MATERIAL" && cmd.Params.Count <= 1)
				{
					if (cmd.Params.Count == 0)
						mesh = this.GetMeshByMaterialID(string.Empty, true);
					else if (materialList.ContainsID(cmd.Params[0]))
					{
						mesh = this.GetMeshByMaterialID(cmd.Params[0], true);
					}
				}
				if (cmd.Name == "VERTEX3" && cmd.Params.Count == 3)
				{
					if (mesh != null)
						mesh.AddVertex(new MeshVertex() { Position = cmd.Params.ToVector3(), Normal = normal, Color = color, TexCoord = texCoord });
				}
				if (cmd.Name == "NORMAL3" && cmd.Params.Count == 3)
				{
					normal = cmd.Params.ToVector3();
				}
				if (cmd.Name == "COLOR4" && cmd.Params.Count == 4)
				{
					color = cmd.Params.ToColor4();
				}
				if (cmd.Name == "TEXCOORD2" && cmd.Params.Count == 2)
				{
					texCoord = cmd.Params.ToVector2();
				}
				if (cmd.Name == "PLANE" && cmd.Params.Count == 4)
				{
					if (mesh != null)
						mesh.AddPlane(cmd.Params[0].ToClearFloat(), cmd.Params[1].ToClearFloat(), cmd.Params[2].ToClearInt(), cmd.Params[3].ToClearInt(), color);
				}
			}
		}
	}
}
