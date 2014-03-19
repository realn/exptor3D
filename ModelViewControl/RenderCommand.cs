using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModelEditor
{
	public class RenderCommand
	{
		public string Name { get; set; }
		public List<string> Params { get; set; }

		public RenderCommand()
		{
			Name = string.Empty;
			Params = new List<string>();
		}
		public RenderCommand(string command) :
			this()
		{
			string clean = command.Trim();
			int pos = clean.IndexOf("(");
			int endpos = clean.IndexOf(")");

			if (pos < 0 || endpos < 0)
				return;

			Name = clean.Substring(0, pos).Trim();
			pos++;

			var list = clean.Substring(pos, endpos - pos).Split(new[] { "," }, StringSplitOptions.RemoveEmptyEntries).ToList();
			foreach (var par in list)
			{
				Params.Add(par.Trim());
			}
		}
	}
}
