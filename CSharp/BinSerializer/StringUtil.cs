using System.Collections.Generic;
using System.Text;

class StringUtil {
	public static string binToHex(List<byte> data) {
		const string hex = "0123456789ABCDEF";
		var result = new StringBuilder();

		int lineCount = (data.Count + 15) / 16;

		for (int line = 0; line < lineCount; line++) {
			int lineStart = line * 16;
			for (int i = 0; i < 16; i++) {
				int index = lineStart + i;

				if (index < data.Count) {
					byte v = data[index];
					result.Append((char)hex[v >> 4]);
					result.Append((char)hex[v & 0xF]);
				} else {
					result.Append("  ");
				}
				result.Append(' ');
			}

			result.Append("    ");

			for (int i = 0; i < 16; i++) {
				int index = lineStart + i;

				if (index < data.Count) {
					byte v = data[index];
					if (v < 32) {
						result.Append(' '); // non-printable character
					} else {
						result.Append((char)v);
					}
				} else {
					result.Append(' ');
				}
	//			result.Append(' ');
			}
			result.Append('\n');
		}
		return result.ToString();
	}
}