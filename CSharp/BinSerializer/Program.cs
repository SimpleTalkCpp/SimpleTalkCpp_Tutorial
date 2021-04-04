using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;

class MyProgram {

	static void test_fixed_byte(ulong n, ulong step) {
		Console.WriteLine("---- test_fixed_byte ----");
		var buf = new List<byte>();

		{
			var se = new BinSerializer(buf);
			for (ulong i = 0; i < n; i+=step) {
				byte t = (byte)i;
				se.io_fixed(ref t);
			}
		}

//		Console.WriteLine(StringUtil.binToHex(buf));

		{
			var se = new BinDeserializer(buf);
			for (ulong i = 0; i < n; i+=step) {
				byte expected = (byte)i;
				byte t = 0;
				se.io_fixed(ref t);
				Debug.Assert(t == expected);
			}
		}
	}

	static void test_fixed_ushort(ulong n, ulong step) {
		Console.WriteLine("---- test_fixed_ushort ----");

		var buf = new List<byte>();

		{
			var se = new BinSerializer(buf);
			for (ulong i = 0; i < n; i+=step) {
				ushort t = (ushort)i;
				se.io_fixed(ref t);
			}
		}

//		Console.WriteLine(StringUtil.binToHex(buf));

		{
			var se = new BinDeserializer(buf);
			for (ulong i = 0; i < n; i+=step) {
				ushort expected = (ushort)i;
				ushort t = 0;
				se.io_fixed(ref t);
				Debug.Assert(t == expected);
			}
		}
	}

	static void test_fixed_float(ulong n, float step) {
		Console.WriteLine("---- test_fixed_float ----");

		var buf = new List<byte>();

		{
			var se = new BinSerializer(buf);
			for (ulong i = 0; i < n; i++) {
				float t = (float)i * step;
				se.io(ref t);
			}
		}

//		Console.WriteLine(StringUtil.binToHex(buf));

		{
			var se = new BinDeserializer(buf);
			for (ulong i = 0; i < n; i++) {
				float expected = (float)i * step;
				float t = 0;
				se.io(ref t);
				Debug.Assert(t == expected);
			}
		}
	}

	static void test_vary_sbyte(ulong n, ulong step) {
		Console.WriteLine("---- test_vary_sbyte ----");

		var buf = new List<byte>();
		{
			var se = new BinSerializer(buf);
			for (ulong i = 0; i < n; i += step) {
				sbyte t = (sbyte)i;
				se.io(ref t);
			}
		}

//		Console.WriteLine(StringUtil.binToHex(buf));

		{
			var se = new BinDeserializer(buf);
			for (ulong i = 0; i < n; i += step) {
				sbyte expected = (sbyte)i;
				sbyte t = 0;
				se.io(ref t);
				Debug.Assert(t == expected);
			}
		}
	}

	static void test_vary_short(ulong n, ulong step) {
		Console.WriteLine("---- test_vary_short ----");

		var buf = new List<byte>();
		{
			var se = new BinSerializer(buf);
			for (ulong i = 0; i < n; i += step) {
				short t = (short)i;
				se.io(ref t);
			}
		}

//		Console.WriteLine(StringUtil.binToHex(buf));

		{
			var se = new BinDeserializer(buf);
			for (ulong i = 0; i < n; i += step) {
				short expected = (short)i;
				short t = 0;
				se.io(ref t);
				Debug.Assert(t == expected);
			}
		}
	}

	static void test_string() {
		Console.WriteLine("---- test_string ----");
		string srcHello = "hello";
		string srcEmpty = "";
		string srcNull  = null;

		var buf = new List<byte>();
		var se = new BinSerializer(buf);
		se.io(ref srcHello);
		se.io(ref srcEmpty);
		se.io(ref srcNull );

		Console.WriteLine(StringUtil.binToHex(buf));

		var de = new BinDeserializer(buf);

		string dstHello = null;
		string dstEmpty = null;
		string dstNull  = null;

		de.io(ref dstHello);
		de.io(ref dstEmpty);
		de.io(ref dstNull );

		Debug.Assert(srcHello == dstHello);
		Debug.Assert(srcEmpty == dstEmpty);
		Debug.Assert(""       == dstNull );
	}

	struct MyStruct {
		public int i;
		public float f;
	}

	class MyClass {
		public int i;
		public float f;
	}

	static void test_struct() {
		Console.WriteLine("---- test_struct ----");

		var src = new MyStruct();
		src.i = 100;
		src.f = 1.2f;

		var buf = new List<byte>();
		var se = new BinSerializer(buf);
		se.io(ref src);

		Console.WriteLine(StringUtil.binToHex(buf));

		var de = new BinDeserializer(buf);
		var dst = new MyStruct();
		de.io(ref dst);

		Debug.Assert(src.i == dst.i);
		Debug.Assert(src.f == dst.f);
	}

	static void test_struct_array() {
		Console.WriteLine("---- test_struct_array ----");

		var srcList = new MyStruct[2];
		for (int i =0 ; i < srcList.Length; i++) {
			srcList[i] = new MyStruct();
			srcList[i].i = 100 * i;
			srcList[i].f = 1.2f * i;
		}

		var buf = new List<byte>();
		var se = new BinSerializer(buf);
		se.io_object(srcList);

		Console.WriteLine(StringUtil.binToHex(buf));

		var dstList = new MyStruct[0];

		var de = new BinDeserializer(buf);
		de.io(ref dstList);

		for (int i = 0; i < srcList.Length; i++) {
			Debug.Assert(srcList[i].i == dstList[i].i);
			Debug.Assert(srcList[i].f == dstList[i].f);
		}
	}

	static void test_class_array() {
		Console.WriteLine("---- test_class_array ----");

		var srcList = new MyClass[2];
		for (int i =0 ; i < srcList.Length; i++) {
			srcList[i] = new MyClass();
			srcList[i].i = 100 * i;
			srcList[i].f = 1.2f * i;
		}

		var buf = new List<byte>();
		var se = new BinSerializer(buf);
		se.io_object(srcList);

		Console.WriteLine(StringUtil.binToHex(buf));

		var dstList = new MyClass[0];

		var de = new BinDeserializer(buf);
		de.io(ref dstList);

		for (int i = 0; i < srcList.Length; i++) {
			Debug.Assert(srcList[i].i == dstList[i].i);
			Debug.Assert(srcList[i].f == dstList[i].f);
		}
	}

	static void test_struct_list() {
		Console.WriteLine("---- test_struct_list ----");

		var srcList = new List<MyStruct>();

		var src = new MyStruct();
		src.i = 100;
		src.f = 1.2f;

		for (int i =0 ; i < 2; i++) {
			srcList.Add(src);
		}

		var buf = new List<byte>();
		var se = new BinSerializer(buf);
		se.io_object(srcList);

		Console.WriteLine(StringUtil.binToHex(buf));

		var dstList = new List<MyStruct>();

		var de = new BinDeserializer(buf);
		de.io(ref dstList);

		for (int i = 0; i < srcList.Count; i++) {
			Debug.Assert(srcList[i].i == dstList[i].i);
			Debug.Assert(srcList[i].f == dstList[i].f);
		}
	}

	static void test_dict() {
		Console.WriteLine("---- test_dict ----");

		var srcList = new Dictionary<int, int>();

		for (int i =0 ; i < 20; i++) {
			srcList.Add(i, i * 10);
		}

		var buf = new List<byte>();
		var se = new BinSerializer(buf);
		se.io_object(srcList);

		Console.WriteLine(StringUtil.binToHex(buf));

		var dstList = new Dictionary<int, int>();

		var de = new BinDeserializer(buf);
		de.io(ref dstList);

		for (int i = 0; i < srcList.Count; i++) {
			Debug.Assert(srcList[i] == dstList[i]);
			Debug.Assert(srcList[i] == dstList[i]);
		}
	}

	static void Main(string[] args) {
		test_string();
		test_dict();

		test_struct_array();
		test_class_array();

		test_struct_list();
		test_struct();

		test_fixed_float(0x100000, 0.1f);
		test_fixed_float(0x100000, 110.1f);

		test_fixed_byte(0x100, 1);
		test_fixed_ushort(0x10000, 1);

		test_vary_sbyte(0x100,   1);
		test_vary_short(0x10000, 1);
	}
}

