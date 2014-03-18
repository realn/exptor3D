using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using System.Runtime.InteropServices;

namespace ModelEditor
{
	public partial class ModelControl : GLControl
	{

		private struct LineVertex
		{
			public Vector3 Position { get; set; }
			public Color4 Color { get; set; }
		}

		LineVertex[] GridArray;
		int GridBuffer;
		ModelObject modelObject;

		public ModelControl() :
			base(new OpenTK.Graphics.GraphicsMode(new ColorFormat(32), 24, 8), 1, 4, GraphicsContextFlags.Default)
		{
			InitializeComponent();
		}

		protected override void OnHandleCreated(EventArgs e)
		{
			base.OnHandleCreated(e);

			if (!this.Context.IsCurrent)
				this.Context.MakeCurrent(this.WindowInfo);

			GL.ClearColor(Color.DarkBlue);
			GL.ClearDepth(1.0f);
			GL.Disable(EnableCap.Lighting);
			GL.Disable(EnableCap.Texture2D);
			GL.Enable(EnableCap.ColorMaterial);
			GL.Disable(EnableCap.CullFace);

			List<LineVertex> verts = new List<LineVertex>();

			Color4 color = new Color4(1.0f, 0.0f, 0.0f, 1.0f);

			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, 1.0f, -1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(1.0f, 1.0f, -1.0f), Color = color });

			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, -1.0f, -1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(1.0f, -1.0f, -1.0f), Color = color });

			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, 1.0f, 1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(1.0f, 1.0f, 1.0f), Color = color });

			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, -1.0f, 1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(1.0f, -1.0f, 1.0f), Color = color });

			color = new Color4(0.0f, 1.0f, 0.0f, 1.0f);

			verts.Add(new LineVertex() { Position = new Vector3(1.0f, -1.0f, 1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(1.0f, 1.0f, 1.0f), Color = color });

			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, -1.0f, 1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, 1.0f, 1.0f), Color = color });

			verts.Add(new LineVertex() { Position = new Vector3(1.0f, -1.0f, -1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(1.0f, 1.0f, -1.0f), Color = color });

			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, -1.0f, -1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, 1.0f, -1.0f), Color = color });

			color = new Color4(0.0f, 0.0f, 1.0f, 1.0f);

			verts.Add(new LineVertex() { Position = new Vector3(1.0f, 1.0f, 1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(1.0f, 1.0f, -1.0f), Color = color });

			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, 1.0f, 1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, 1.0f, -1.0f), Color = color });

			verts.Add(new LineVertex() { Position = new Vector3(1.0f, -1.0f, 1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(1.0f, -1.0f, -1.0f), Color = color });

			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, -1.0f, 1.0f), Color = color });
			verts.Add(new LineVertex() { Position = new Vector3(-1.0f, -1.0f, -1.0f), Color = color });

			this.GridArray = verts.ToArray();

			this.GridBuffer = GL.GenBuffer();
			GL.BindBuffer(BufferTarget.ArrayBuffer, this.GridBuffer);
			int size = Marshal.SizeOf(typeof(LineVertex));
			GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(size * this.GridArray.Length), this.GridArray, BufferUsageHint.StaticDraw);
			GL.BindBuffer(BufferTarget.ArrayBuffer, 0);

			modelObject = new ModelObject();
		}

		private void RenderGrid()
		{
			float asp = (float)this.Width / (float)this.Height;
			var mat = Matrix4.CreatePerspectiveFieldOfView(MathHelper.DegreesToRadians(60.0f), asp, 0.3f, 100.0f);

			GL.MatrixMode(MatrixMode.Projection);
			GL.LoadMatrix(ref mat);
			GL.MatrixMode(MatrixMode.Modelview);

			mat = Matrix4.CreateTranslation(0.0f, 0.0f, -3.0f);
			GL.LoadMatrix(ref mat);

			GL.EnableClientState(ArrayCap.VertexArray);
			GL.EnableClientState(ArrayCap.ColorArray);

			int size = Marshal.SizeOf(typeof(LineVertex));
			GL.BindBuffer(BufferTarget.ArrayBuffer, this.GridBuffer);
			GL.VertexPointer(3, VertexPointerType.Float, size, 0);
			GL.ColorPointer(4, ColorPointerType.Float, size, Marshal.SizeOf(typeof(Vector3)));

			GL.LineWidth(2.0f);
			GL.DrawArrays(PrimitiveType.Lines, 0, this.GridArray.Length);

			GL.DisableClientState(ArrayCap.ColorArray);
			GL.EnableClientState(ArrayCap.VertexArray);
		}

		public void Render()
		{
			if (!this.Context.IsCurrent)
				this.Context.MakeCurrent(this.WindowInfo);

			GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

			this.RenderGrid();

			modelObject.Render();

			this.SwapBuffers();
		}

		public void ParseSource(string source)
		{
			var instList = source.Split(new[] { ";" }, StringSplitOptions.RemoveEmptyEntries);

			var commandList = new List<RenderCommand>();

			foreach (var inst in instList)
			{
				if (string.IsNullOrWhiteSpace(inst))
					continue;
				commandList.Add(new RenderCommand(inst.Trim()));
			}

			if (modelObject.MeshList.Count == 0)
				modelObject.MeshList.Add(new Mesh(PrimitiveType.Triangles));

			modelObject.MeshList[0].ParseCommands(commandList);
			modelObject.UpdateMeshes();
		}
	}
}
