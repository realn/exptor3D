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

namespace ModelEditor
{
    public partial class ModelControl : GLControl
    {
        public ModelControl() :
            base( new OpenTK.Graphics.GraphicsMode(new ColorFormat( 32 ), 24, 8), 1, 4, GraphicsContextFlags.Default)
        {
            InitializeComponent();
        }

        private void RenderGrid()
        {
            float asp = (float)this.Width / (float)this.Height;
            var mat = Matrix4.CreatePerspectiveFieldOfView(MathHelper.DegreesToRadians(60.0f), asp, 0.3f, 100.0f);

            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadMatrix(ref mat);
            GL.MatrixMode(MatrixMode.Modelview);
            GL.LoadIdentity();

            GL.Begin(BeginMode.Lines);
            GL.Color3(1, 1, 1);
            GL.Vertex3(-1.0f, 0.0f, -1.0f);
            GL.Vertex3(1.0f, 0.0f, -1.0f);
            GL.End();

        }

        public void Render()
        {
            GL.ClearDepth(1.0f);
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            this.RenderGrid();

            this.SwapBuffers();
        }
    }
}
