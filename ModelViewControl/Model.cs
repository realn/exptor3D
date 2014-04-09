using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModelViewControl
{
	public class Model
	{
		public string Name { get; set; }
		public List<ModelObject> ObjectList { get; private set; }

		public Model(string name)
		{
			this.ObjectList = new List<ModelObject>();
		}
	}
}
