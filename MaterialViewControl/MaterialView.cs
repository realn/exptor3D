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

namespace MaterialViewControl
{
	[Serializable]
	public partial class MaterialView : UserControl
	{
		private Point mousePos = new Point();
		private int dragging = 0;
		private Vector2 viewRotation = new Vector2();
		private float zoom = 3.0f;
		private bool loaded = false;

		public Material Material { get; set; }

		public MaterialView()
		{
			InitializeComponent();
		}

		public void Render()
		{
			if (!this.GLControl.Context.IsCurrent)
				this.GLControl.Context.MakeCurrent(this.GLControl.WindowInfo);

			GL.ClearColor(new Color4(0.0f, 0.0f, 1.0f, 1.0f));
			GL.ClearDepth(1.0f);
			GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

			float asp = (float)this.GLControl.Width / (float)this.GLControl.Height;
			var proj = Matrix4.CreatePerspectiveFieldOfView(MathHelper.DegreesToRadians(60.0f), asp, 0.5f, 100.0f);
			var view =
				Matrix4.CreateRotationY(MathHelper.DegreesToRadians(viewRotation.Y)) *
				Matrix4.CreateRotationX(MathHelper.DegreesToRadians(viewRotation.X)) *
				Matrix4.CreateTranslation(0.0f, 0.0f, -this.zoom);

			GL.MatrixMode(MatrixMode.Projection);
			GL.LoadMatrix(ref proj);
			GL.MatrixMode(MatrixMode.Modelview);
			GL.LoadMatrix(ref view);

			if (this.Material != null)
			{
				GL.Color4(this.Material.Diffuse.Color);
				if (this.Material.Diffuse.HasTexture)
				{
					GL.Enable(EnableCap.Texture2D);
					GL.BindTexture(TextureTarget.Texture2D, this.Material.Diffuse.Texture.GLID);
				}
				else
				{
					GL.Disable(EnableCap.Texture2D);
					GL.BindTexture(TextureTarget.Texture2D, 0);
				}

				GL.Begin(PrimitiveType.Quads);

				GL.TexCoord2(0.0f, 0.0f);
				GL.Vertex2(-1.0f, -1.0f);

				GL.TexCoord2(1.0f, 0.0f);
				GL.Vertex2(1.0f, -1.0f);

				GL.TexCoord2(1.0f, 1.0f);
				GL.Vertex2(1.0f, 1.0f);

				GL.TexCoord2(0.0f, 1.0f);
				GL.Vertex2(-1.0f, 1.0f);

				GL.End();
			}

			this.GLControl.SwapBuffers();
		}

		private void GLControl_MouseDown(object sender, MouseEventArgs e)
		{
			if (
				(e.Button & MouseButtons.Left) == System.Windows.Forms.MouseButtons.Left ||
				(e.Button & MouseButtons.Right) == System.Windows.Forms.MouseButtons.Right)
			{
				this.dragging++;
				this.mousePos = new Point(e.X, e.Y);
			}
		}

		private void GLControl_MouseMove(object sender, MouseEventArgs e)
		{
			if (this.dragging > 0)
			{
				var point = new Point(e.X, e.Y);
				if ((e.Button & System.Windows.Forms.MouseButtons.Left) == System.Windows.Forms.MouseButtons.Left)
				{
					viewRotation.X += (float)(this.mousePos.Y - point.Y);
					viewRotation.Y += (float)(this.mousePos.X - point.X);
				}
				if ((e.Button & System.Windows.Forms.MouseButtons.Right) == System.Windows.Forms.MouseButtons.Right)
				{
					zoom = Math.Max(3.0f, zoom + (float)(this.mousePos.Y - point.Y) / 10.0f);
				}
				this.mousePos = point;

				this.GLControl.Invalidate();
			}
		}

		private void GLControl_MouseUp(object sender, MouseEventArgs e)
		{
			if (
				(e.Button & MouseButtons.Left) == System.Windows.Forms.MouseButtons.Left ||
				(e.Button & MouseButtons.Right) == System.Windows.Forms.MouseButtons.Right)
				this.dragging--;
		}

		private void GLControl_Paint(object sender, PaintEventArgs e)
		{
			if (this.loaded)
				this.Render();
		}

		private void MaterialView_Load(object sender, EventArgs e)
		{
			this.loaded = true;
		}
	}
}
