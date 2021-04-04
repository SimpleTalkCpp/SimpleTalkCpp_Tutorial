using System;

namespace Ex001
{
	class Program
	{
		static void Main(string[] args)
		{
            int requireHeight = 10;

            int y = 0;
            while (y < requireHeight) {
                int x = 0;
                while (x < requireHeight) {
                    if (x >= y) {
                        Console.Write("*");
                    } else {
                        Console.Write(" ");
                    }
                    x++;
                }
                Console.WriteLine("");
                y++;
            }
		}
	}
}
