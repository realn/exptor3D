using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using System.Globalization;

namespace ModelEditor
{
	public class ModelObject
	{
		public string ID { get; set; }
		public List<Mesh> MeshList { get; set; }

		public ModelObject()
		{
			this.MeshList = new List<Mesh>();
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
	}
}
