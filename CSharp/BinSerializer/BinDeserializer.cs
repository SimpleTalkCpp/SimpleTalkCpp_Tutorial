using System.Collections.Generic;
using System.Text;

class BinDeserializer {

	public BinDeserializer(List<byte> buf) {
		_buf = buf;
		_index = 0;
	}

	public void io_fixed(ref byte value) {
		value = _getByte();
	}

	public void io_fixed(ref ushort value) {
		value  = (ushort)(_getByte()     );
		value |= (ushort)(_getByte() << 8);
	}

	public void io_fixed(ref uint value) {
		value  = (uint)(_getByte()      );
		value |= (uint)(_getByte() << 8 );
		value |= (uint)(_getByte() << 16);
		value |= (uint)(_getByte() << 24);
	}

	public void io_fixed(ref ulong value) {
		value  = (ulong)_getByte();
		value |= (ulong)_getByte() << 8;
		value |= (ulong)_getByte() << 16;
		value |= (ulong)_getByte() << 24;
		value |= (ulong)_getByte() << 32;
		value |= (ulong)_getByte() << 40;
		value |= (ulong)_getByte() << 48;
		value |= (ulong)_getByte() << 56;
	}

	public void io_fixed(ref sbyte value) {
		value = (sbyte)_getByte();
	}

	public void io_fixed(ref short value) {
		value  = (short)(_getByte()     );
		value |= (short)(_getByte() << 8);
	}

	public void io_fixed(ref int value) {
		value  = (int)(_getByte()      );
		value |= (int)(_getByte() << 8 );
		value |= (int)(_getByte() << 16);
		value |= (int)(_getByte() << 24);
	}

	public void io_fixed(ref long value) {
		value  = (long)_getByte();
		value |= (long)_getByte() << 8;
		value |= (long)_getByte() << 16;
		value |= (long)_getByte() << 24;
		value |= (long)_getByte() << 32;
		value |= (long)_getByte() << 40;
		value |= (long)_getByte() << 48;
		value |= (long)_getByte() << 56;
	}

	public void io(ref float value) {
		var tmp = new IntToFloatUnion();
		io_fixed(ref tmp.i);
		value = tmp.f;
	}

	public void io(ref double value) {
		var tmp = new LongToDoubleUnion();
		io_fixed(ref tmp.i);
		value = tmp.f;
	}

	public void io(ref byte value) {
		int bit = 0;
		value = 0;
		for(;;){
			byte t = _getByte();
			value |= (byte)((t & 0x7F) << bit);
			if((t & 0x80) == 0) return;
			bit += 7;
			if( bit > sizeof(byte) * 8 ) {
				throw new Error("BinDeserializer out of range");
			}
		}
	}

	public void io(ref ushort value) {
		int bit = 0;
		value = 0;
		for(;;){
			ushort t = _getByte();
			value |= (ushort)((t & 0x7F) << bit);
			if((t & 0x80) == 0) return;
			bit += 7;
			if( bit > sizeof(ushort) * 8 ) {
				throw new Error("BinDeserializer out of range");
			}
		}
	}

	public void io(ref uint value) {
		int bit = 0;
		value = 0;
		for(;;){
			uint t = _getByte();
			value |= (uint)((t & 0x7F) << bit);
			if((t & 0x80) == 0) return;
			bit += 7;
			if( bit > sizeof(uint) * 8 ) {
				throw new Error("BinDeserializer out of range");
			}
		}
	}

	public void io(ref ulong value) {
		int bit = 0;
		value = 0;
		for(;;){
			ulong t = _getByte();
			value |= (ulong)((t & 0x7F) << bit);
			if((t & 0x80) == 0) return;
			bit += 7;
			if( bit > sizeof(ulong) * 8 ) {
				throw new Error("BinDeserializer out of range");
			}
		}
	}

	// ZigZag encoding - https://developers.google.com/protocol-buffers/docs/encoding
	public void io(ref sbyte value) { byte   tmp = 0; io(ref tmp); value = (sbyte)((tmp >> 1) ^ ( -(tmp & 1))); }
	public void io(ref short value) { ushort tmp = 0; io(ref tmp); value = (short)((tmp >> 1) ^ ( -(tmp & 1))); }
	public void io(ref int   value) { uint   tmp = 0; io(ref tmp); value = (int  )((tmp >> 1) ^ ( -(tmp & 1))); }
	public void io(ref long  value) { ulong  tmp = 0; io(ref tmp); value = (long )(tmp >> 1) ^ ( -((long)tmp & 1L)); }

	public void io(ref char value) { ushort tmp = 0; io(ref tmp); value = (char)tmp; }

	public void io(ref string value) {
		int len = 0;
		io(ref len);
		var arr = new char[len];
		for (int i = 0; i < len; i++) {
			io(ref arr[i]);
		}
		value = new string(arr);
	}

	public void io<T>(ref T value) {
		var type = typeof(T);
		object obj = value;
		io_object(ref obj);
		value = (T)obj;
	}

	public void io_object(ref object obj) {
		var type = obj.GetType();
		if (type == typeof(byte  )) { byte   tmp = 0; io(ref tmp); obj = tmp; return; }
		if (type == typeof(ushort)) { ushort tmp = 0; io(ref tmp); obj = tmp; return; }
		if (type == typeof(uint  )) { uint   tmp = 0; io(ref tmp); obj = tmp; return; }
		if (type == typeof(ulong )) { ulong  tmp = 0; io(ref tmp); obj = tmp; return; }
									  
		if (type == typeof(sbyte )) { sbyte  tmp = 0; io(ref tmp); obj = tmp; return; }
		if (type == typeof(short )) { short  tmp = 0; io(ref tmp); obj = tmp; return; }
		if (type == typeof(int   )) { int    tmp = 0; io(ref tmp); obj = tmp; return; }
		if (type == typeof(long  )) { long   tmp = 0; io(ref tmp); obj = tmp; return; }

		if (type == typeof(float )) { float  tmp = 0; io(ref tmp); obj = tmp; return; }
		if (type == typeof(double)) { double tmp = 0; io(ref tmp); obj = tmp; return; }

		if (type == typeof(string)) { string tmp ="";   io(ref tmp); obj = tmp; return; }
		if (type == typeof(char  )) { char   tmp ='\0'; io(ref tmp); obj = tmp; return; }

		if (type.IsArray) {
			int len = 0;
			io(ref len);

			var elemType = type.GetElementType();
			var arr = System.Array.CreateInstance(elemType, len);

			for (int i = 0; i < len; i++) {
				var elemValue = System.Activator.CreateInstance(elemType);
				io_object(ref elemValue);
				arr.SetValue(elemValue, i);
			}
			obj = arr;
			return;
		}

		var list = obj as System.Collections.IList;
		if (list != null) {
			list.Clear();

			var elemType = type.GenericTypeArguments[0];

			int len = 0;
			io(ref len);
			for (int i = 0; i < len; i++) {
				var elemValue = System.Activator.CreateInstance(elemType);
				io_object(ref elemValue);
				list.Add(elemValue);
			}
			return;
		}

		var dict = obj as System.Collections.IDictionary;
		if (dict != null) {
			int len = 0;
			io(ref len);

			var keyType   = type.GenericTypeArguments[0];
			var valueType = type.GenericTypeArguments[1];

			for (int i = 0; i < len; i++) {
				var key   = System.Activator.CreateInstance(keyType);
				var value = System.Activator.CreateInstance(valueType);
				io_object(ref key);
				io_object(ref value);
				dict.Add(key, value);
			}
			return;
		}

		var fields = type.GetFields();
		foreach (var f in fields) {
			var fieldValue = System.Activator.CreateInstance(f.FieldType);
			io_object(ref fieldValue);
			f.SetValue(obj, fieldValue);
		}
	}

	byte _getByte() {
		if (_index >= _buf.Count) {
			throw new Error("BinDeserializer out of range");
		}
		return _buf[_index++];
	}

	List<byte> _buf; // TODO - use Span<byte> instead
	int _index;
}