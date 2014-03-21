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
		private bool dragging = false;
		private Vector2 viewRotation = new Vector2();

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
			var view = Matrix4.CreateTranslation(0.0f, 0.0f, -2.0f) *
				Matrix4.CreateRotationY(MathHelper.DegreesToRadians(viewRotation.Y)) *
				Matrix4.CreateRotationX(MathHelper.DegreesToRadians(viewRotation.X));

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
			if (e.Button == System.Windows.Forms.MouseButtons.Left)
			{
				this.dragging = true;
				this.mousePos = new Point(e.X, e.Y);
			}
		}

		private void GLControl_MouseMove(object sender, MouseEventArgs e)
		{
			if (this.dragging)
			{
				var point = new Point(e.X, e.Y);
				viewRotation.X += (float)(this.mousePos.Y - point.Y);
				viewRotation.Y += (float)(this.mousePos.X - point.X);
				this.mousePos = point;
			}
		}

		private void GLControl_MouseUp(object sender, MouseEventArgs e)
		{
			if (e.Button == System.Windows.Forms.MouseButtons.Left)
				this.dragging = false;
		}
	}
}
