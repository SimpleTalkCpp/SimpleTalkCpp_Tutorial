using System;

namespace Ex002
{
	class Program
	{
		static void Main(string[] args)
		{
			int requireHeight = 32;
			int reducePerLayer = 3;
			int layerHeight = 8;
			int decorationPerLeaf = 7;

			int leafPerRow = 1;
			int leafId = 0;


			int padding = requireHeight - (requireHeight - 1) / layerHeight * reducePerLayer;

			for (int y = 0; y < requireHeight; y++) {
				// white spaces
				for (int x = 0; x < padding - (leafPerRow / 2 - 1); x++) {
					Console.Write(" ");
				}

				// stars
				for (int x = 0; x < leafPerRow; x++) {
					if (leafId % decorationPerLeaf == 0) {
						Console.Write("O");
					} else {
						Console.Write("+");
					}
					leafId++;
				}

				if (y % layerHeight == layerHeight - 1) {
					leafPerRow -= reducePerLayer * 2;
				} else {
					leafPerRow += 2;
				}

				Console.WriteLine("");
			}

			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < padding; x++) {
					Console.Write(" ");
				}
				Console.WriteLine("|#|");
			}
		}
	}
}
