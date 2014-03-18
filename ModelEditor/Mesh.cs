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
	internal struct MeshVertex
	{
		public Vector3 Position { get; set; }
		public Vector3 Normal { get; set; }
		public Vector2 TexCoord { get; set; }
		public Color4 Color { get; set; }

		public static bool operator ==(MeshVertex vert1, MeshVertex vert2)
		{
			return vert1.Position == vert2.Position && vert1.Normal == vert2.Normal && vert1.TexCoord == vert2.TexCoord && vert1.Color == vert2.Color;
		}
		public static bool operator !=(MeshVertex vert1, MeshVertex vert2)
		{
			return !(vert1.Position == vert2.Position && vert1.Normal == vert2.Normal && vert1.TexCoord == vert2.TexCoord && vert1.Color == vert2.Color);
		}

		public override bool Equals(object obj)
		{
			return base.Equals(obj);
		}
		public override int GetHashCode()
		{
			return base.GetHashCode();
		}
	}

	internal class Mesh : IDisposable
	{
		public List<MeshVertex> Vertices { get; set; }
		public List<ushort> Indices { get; set; }
		public string MaterialID { get; set; }
		public PrimitiveType PrimitiveType { get; set; }

		private int vertexBuffer;
		private int indexBuffer;

		public Mesh(PrimitiveType type)
		{
			PrimitiveType = type;
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

		public void AddPlane(float width, float height, int slicesX, int slicesY, Color4 color)
		{
			float stepX = width / (float)(slicesX + 1);
			float stepY = height / (float)(slicesY + 1);


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
			GL.DrawElements(this.PrimitiveType, Indices.Count, DrawElementsType.UnsignedShort, 0);

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
			}
		}
	}
}
