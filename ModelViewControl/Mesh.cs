using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using System.Runtime.InteropServices;

namespace ModelEditor
{
	public struct MeshVertex
	{
		public Vector3 Position { get; set; }
		public Vector3 Normal { get; set; }
		public Vector2 TexCoord { get; set; }
		public Color4 Color { get; set; }

		#region Public Methods

		public override bool Equals(object obj)
		{
			return base.Equals(obj);
		}
		public override int GetHashCode()
		{
			return base.GetHashCode();
		}

		#endregion

		public static bool operator ==(MeshVertex vert1, MeshVertex vert2)
		{
			return vert1.Position == vert2.Position && vert1.Normal == vert2.Normal && vert1.TexCoord == vert2.TexCoord && vert1.Color == vert2.Color;
		}
		public static bool operator !=(MeshVertex vert1, MeshVertex vert2)
		{
			return !(vert1.Position == vert2.Position && vert1.Normal == vert2.Normal && vert1.TexCoord == vert2.TexCoord && vert1.Color == vert2.Color);
		}
	}

	public class Mesh : IDisposable
	{
		public List<MeshVertex> Vertices { get; set; }
		public List<ushort> Indices { get; set; }
		public string MaterialID { get; set; }
		public PrimitiveType RenderPrimitive { get; set; }

		private int vertexBuffer;
		private int indexBuffer;

		public Mesh(PrimitiveType type)
		{
			RenderPrimitive = type;
			Vertices = new List<MeshVertex>();
			Indices = new List<ushort>();

			this.vertexBuffer = GL.GenBuffer();
			this.indexBuffer = GL.GenBuffer();
		}

		public void AddVertex(MeshVertex vertex)
		{
			var index = this.Vertices.FindIndex((vert) => { return vert == vertex; });
			if (index < 0)
			{
				index = this.Vertices.Count;
				this.Vertices.Add(vertex);
			}
			this.Indices.Add((ushort)index);
		}

		public void AddLine(MeshVertex v1, MeshVertex v2)
		{
			if (RenderPrimitive == PrimitiveType.Triangles)
				return;

			AddVertex(v1);
			AddVertex(v2);
		}

		public void AddQuad(MeshVertex v1, MeshVertex v2, MeshVertex v3, MeshVertex v4)
		{
			switch (RenderPrimitive)
			{
				case PrimitiveType.Points:
					AddVertex(v1);
					AddVertex(v2);
					AddVertex(v3);
					AddVertex(v4);
					break;

				case PrimitiveType.Lines:
					AddLine(v1, v2);
					AddLine(v2, v3);
					AddLine(v3, v4);
					AddLine(v4, v1);
					break;

				case PrimitiveType.Triangles:
					AddVertex(v1);
					AddVertex(v2);
					AddVertex(v3);

					AddVertex(v1);
					AddVertex(v3);
					AddVertex(v4);
					break;
			}
		}

		public void AddPlane(float width, float height, int slicesX, int slicesY, Color4 color)
		{
			float stepX = width / (float)(slicesX + 1);
			float stepY = height / (float)(slicesY + 1);

			Vector3 normal = new Vector3(0.0f, 1.0f, 0.0f);
			for (int y = 0; y < slicesY; y++)
			{
				for (int x = 0; x < slicesX; x++)
				{
					var pos1 = new Vector3((x + 0) * stepX, 0.0f, (y + 0) * stepY);
					var pos2 = new Vector3((x + 1) * stepX, 0.0f, (y + 0) * stepY);
					var pos3 = new Vector3((x + 1) * stepX, 0.0f, (y + 1) * stepY);
					var pos4 = new Vector3((x + 0) * stepX, 0.0f, (y + 1) * stepY);

					var v1 = new MeshVertex() { Position = pos1, TexCoord = new Vector2(0.0f, 0.0f), Color = color, Normal = normal };
					var v2 = new MeshVertex() { Position = pos2, TexCoord = new Vector2(1.0f, 0.0f), Color = color, Normal = normal };
					var v3 = new MeshVertex() { Position = pos3, TexCoord = new Vector2(1.0f, 1.0f), Color = color, Normal = normal };
					var v4 = new MeshVertex() { Position = pos4, TexCoord = new Vector2(0.0f, 1.0f), Color = color, Normal = normal };

					this.AddQuad(v1, v2, v3, v4);
				}
			}
		}

		public void UpdateBuffers(BufferUsageHint usageHint)
		{
			GL.BindBuffer(BufferTarget.ArrayBuffer, this.vertexBuffer);
			GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Marshal.SizeOf(typeof(MeshVertex)) * this.Vertices.Count), this.Vertices.ToArray(), usageHint);
			GL.BindBuffer(BufferTarget.ArrayBuffer, 0);

			GL.BindBuffer(BufferTarget.ElementArrayBuffer, this.indexBuffer);
			GL.BufferData(BufferTarget.ElementArrayBuffer, (IntPtr)(Marshal.SizeOf(typeof(short)) * this.Indices.Count), this.Indices.ToArray(), usageHint);
			GL.BindBuffer(BufferTarget.ElementArrayBuffer, 0);
		}

		public void Render()
		{
			GL.EnableClientState(ArrayCap.VertexArray);
			GL.EnableClientState(ArrayCap.NormalArray);
			GL.EnableClientState(ArrayCap.TextureCoordArray);
			GL.EnableClientState(ArrayCap.ColorArray);

			GL.BindBuffer(BufferTarget.ArrayBuffer, this.vertexBuffer);
			int size = Marshal.SizeOf(typeof(MeshVertex));
			int offset = 0;
			GL.VertexPointer(3, VertexPointerType.Float, size, offset);
			offset += Marshal.SizeOf(typeof(Vector3));
			GL.NormalPointer(NormalPointerType.Float, size, offset);
			offset += Marshal.SizeOf(typeof(Vector3));
			GL.TexCoordPointer(2, TexCoordPointerType.Float, size, offset);
			offset += Marshal.SizeOf(typeof(Vector2));
			GL.ColorPointer(4, ColorPointerType.Float, size, offset);

			GL.BindBuffer(BufferTarget.ElementArrayBuffer, this.indexBuffer);
			GL.DrawElements(this.RenderPrimitive, Indices.Count, DrawElementsType.UnsignedShort, 0);

			GL.DisableClientState(ArrayCap.VertexArray);
			GL.DisableClientState(ArrayCap.NormalArray);
			GL.DisableClientState(ArrayCap.TextureCoordArray);
			GL.DisableClientState(ArrayCap.ColorArray);
		}

		public void Clear()
		{
			this.Vertices.Clear();
			this.Indices.Clear();
			this.MaterialID = string.Empty;
		}

		public void Dispose()
		{
			if (this.vertexBuffer != 0)
			{
				GL.DeleteBuffer(this.vertexBuffer);
				this.vertexBuffer = 0;
			}
			if (this.indexBuffer != 0)
			{
				GL.DeleteBuffer(this.indexBuffer);
				this.indexBuffer = 0;
			}
		}

		public void ParseCommands(IEnumerable<RenderCommand> commandList)
		{
			Clear();

			Vector3 normal = new Vector3(1.0f, 0.0f, 0.0f);
			Vector2 texCoord = new Vector2(0.0f, 0.0f);
			Color4 color = new Color4(1.0f, 1.0f, 1.0f, 1.0f);

			foreach (var cmd in commandList)
			{
				if (cmd.Name == "VERTEX3")
				{
					AddVertex(new MeshVertex() { Position = cmd.Params.ToVector3(), Normal = normal, Color = color, TexCoord = texCoord });
				}
				if (cmd.Name == "NORMAL3")
				{
					normal = cmd.Params.ToVector3();
				}
				if (cmd.Name == "COLOR4")
				{
					color = cmd.Params.ToColor4();
				}
				if (cmd.Name == "TEXCOORD2")
				{
					texCoord = cmd.Params.ToVector2();
				}
				if (cmd.Name == "PLANE")
				{
					this.AddPlane(cmd.Params[0].ToClearFloat(), cmd.Params[1].ToClearFloat(), cmd.Params[2].ToClearInt(), cmd.Params[3].ToClearInt(), color);
				}
			}
		}
	}
}
