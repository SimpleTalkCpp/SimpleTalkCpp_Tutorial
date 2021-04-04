using System.Collections.Generic;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Explicit)] 
struct IntToFloatUnion {
	[FieldOffset(0)] public uint  i;
	[FieldOffset(0)] public float f;
}

[StructLayout(LayoutKind.Explicit)] 
struct LongToDoubleUnion {
	[FieldOffset(0)] public ulong  i;
	[FieldOffset(0)] public double f;
}

class BinSerializer {

	public BinSerializer(List<byte> buf) {
		_buf = buf;
	}

	public void io_fixed(ref byte value) {
		_buf.Add(value);
	}

	public void io_fixed(ref ushort value) {
		_buf.Add((byte)value);
		_buf.Add((byte)(value >> 8));
	}

	public void io_fixed(ref uint value) {
		_buf.Add((byte)value);
		_buf.Add((byte)(value >> 8));
		_buf.Add((byte)(value >> 16));
		_buf.Add((byte)(value >> 24));
	}

	public void io_fixed(ref ulong value) {
		_buf.Add((byte)value);
		_buf.Add((byte)(value >> 8));
		_buf.Add((byte)(value >> 16));
		_buf.Add((byte)(value >> 24));
		_buf.Add((byte)(value >> 32));
		_buf.Add((byte)(value >> 40));
		_buf.Add((byte)(value >> 48));
		_buf.Add((byte)(value >> 56));
	}

	public void io_fixed(ref sbyte value) {
		_buf.Add((byte)value);
	}

	public void io_fixed(ref short value) {
		_buf.Add((byte)value);
		_buf.Add((byte)(value >> 8));
	}

	public void io_fixed(ref int value) {
		_buf.Add((byte)value);
		_buf.Add((byte)(value >> 8));
		_buf.Add((byte)(value >> 16));
		_buf.Add((byte)(value >> 24));
	}

	public void io_fixed(ref long value) {
		_buf.Add((byte)value);
		_buf.Add((byte)(value >> 8));
		_buf.Add((byte)(value >> 16));
		_buf.Add((byte)(value >> 24));
		_buf.Add((byte)(value >> 32));
		_buf.Add((byte)(value >> 40));
		_buf.Add((byte)(value >> 48));
		_buf.Add((byte)(value >> 56));
	}

	public void io(ref float value) {
		var tmp = new IntToFloatUnion();
		tmp.f = value;
		io_fixed(ref tmp.i);
	}

	public void io(ref double value) {
		var tmp = new LongToDoubleUnion();
		tmp.f = value;
		io_fixed(ref tmp.i);
	}

	public void io(ref byte value) {
		byte tmp = value;
		for(;;) {
			byte highBit = tmp >= 0x80 ? (byte)0x80 : (byte)0;
			_buf.Add((byte)(tmp | highBit));
			tmp >>= 7;
			if (tmp == 0) return;
		}
		throw new Error("BinSerializer.io");
	}

	public void io(ref ushort value) {
		ushort tmp = value;
		for(;;) {
			byte highBit = tmp >= 0x80 ? (byte)0x80 : (byte)0;
			_buf.Add((byte)(tmp | highBit));
			tmp >>= 7;
			if (tmp == 0) return;
		}
		throw new Error("BinSerializer.io");
	}

	public void io(ref uint value) {
		uint tmp = value;
		for(;;) {
			byte highBit = tmp >= 0x80 ? (byte)0x80 : (byte)0;
			_buf.Add((byte)(tmp | highBit));
			tmp >>= 7;
			if (tmp == 0) return;
		}
		throw new Error("BinSerializer.io");
	}

	public void io(ref ulong value) {
		ulong tmp = value;
		for(;;) {
			byte highBit = tmp >= 0x80 ? (byte)0x80 : (byte)0;
			_buf.Add((byte)(tmp | highBit));
			tmp >>= 7;
			if (tmp == 0) return;
		}
		throw new Error("BinSerializer.io");
	}

	// ZigZag encoding - https://developers.google.com/protocol-buffers/docs/encoding
	public void io(ref sbyte value) { var tmp = (byte  )((value << 1) ^ (value >> (sizeof(byte )*8-1))); io(ref tmp); }
	public void io(ref short value) { var tmp = (ushort)((value << 1) ^ (value >> (sizeof(short)*8-1))); io(ref tmp); }
	public void io(ref int   value) { var tmp = (uint  )((value << 1) ^ (value >> (sizeof(int  )*8-1))); io(ref tmp); }
	public void io(ref long  value) { var tmp = (ulong )((value << 1) ^ (value >> (sizeof(long )*8-1))); io(ref tmp); }

	public void io(ref char  value) { var tmp = (ushort)value; io(ref tmp); }

	public void io(ref string value) {
		int len = 0;
		if (value != null) {
			len = value.Length;
		}

		io(ref len);
		for (int i = 0; i < len; i++) {
			char c = value[i];
			io(ref c);
		}
	}

	public void io<T>(ref T value) {
		var type = typeof(T);
		object obj = value;
		io_object(obj);
	}

	public void io_object(object obj) {
		var type = obj.GetType();
		if (type == typeof(byte  )) { byte   tmp = (byte  )obj; io(ref tmp); return; }
		if (type == typeof(ushort)) { ushort tmp = (ushort)obj; io(ref tmp); return; }
		if (type == typeof(uint  )) { uint   tmp = (uint  )obj; io(ref tmp); return; }
		if (type == typeof(ulong )) { ulong  tmp = (ulong )obj; io(ref tmp); return; }

		if (type == typeof(sbyte )) { sbyte  tmp = (sbyte )obj; io(ref tmp); return; }
		if (type == typeof(short )) { short  tmp = (short )obj; io(ref tmp); return; }
		if (type == typeof(int   )) { int    tmp = (int   )obj; io(ref tmp); return; }
		if (type == typeof(long  )) { long   tmp = (long  )obj; io(ref tmp); return; }

		if (type == typeof(float )) { float  tmp = (float )obj; io(ref tmp); return; }
		if (type == typeof(double)) { double tmp = (double)obj; io(ref tmp); return; }

		if (type == typeof(string)) { string tmp = (string)obj; io(ref tmp); return; }
		if (type == typeof(char  )) { char   tmp = (char  )obj; io(ref tmp); return; }

		var list = obj as System.Collections.IList;
		if (list != null) {
			int len = list.Count;
			io(ref len);
			for (int i = 0; i < len; i++) {
				io_object(list[i]);
			}
			return;
		}

		var dict = obj as System.Collections.IDictionary;
		if (dict != null) {
			int len = dict.Count;
			io(ref len);

			var it = dict.GetEnumerator();
			while (it.MoveNext()) {
				io_object(it.Key);
				io_object(it.Value);
			}
			return;
		}
	
		var fields = type.GetFields();
		foreach (var f in fields) {
			var fieldValue = f.GetValue(obj);
			io_object(fieldValue);
		}
	}

	List<byte> _buf;
}