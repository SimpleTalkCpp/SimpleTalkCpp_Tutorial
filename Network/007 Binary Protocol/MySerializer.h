#pragma once

#include "my_common.h"

inline uint8_t my_byte_swap(uint8_t v) {
	return v;
}

inline uint16_t my_byte_swap(uint16_t v) {
	union T {
		uint16_t value;
		uint8_t  c[2];
	};
	T src; src.value = v;
	T dst;
	dst.c[0] = src.c[1];
	dst.c[1] = src.c[0];
	return dst.value;
}

inline uint32_t my_byte_swap(uint32_t v) {
	union T {
		uint32_t value;
		uint8_t  c[4];
	};
	T src; src.value = v;
	T dst;
	dst.c[0] = src.c[3];
	dst.c[1] = src.c[2];
	dst.c[2] = src.c[1];
	dst.c[3] = src.c[0];
	return dst.value;
}

inline uint64_t my_byte_swap(uint64_t v) {
	union T {
		uint64_t value;
		uint8_t  c[8];
	};
	T src; src.value = v;
	T dst;
	dst.c[0] = src.c[7];
	dst.c[1] = src.c[6];
	dst.c[2] = src.c[5];
	dst.c[3] = src.c[4];
	dst.c[4] = src.c[3];
	dst.c[5] = src.c[2];
	dst.c[6] = src.c[1];
	dst.c[7] = src.c[0];
	return dst.value;
}

inline uint8_t my_zigzag_encode(int8_t  v) {
	const auto a = static_cast<uint8_t>(v) << 1;  // shift in unsigned
	const auto b = static_cast<uint8_t>(v >> 7); // ! must shift in signed
	return  a ^ b;
}

inline uint16_t my_zigzag_encode(int16_t  v) {
	const auto a = static_cast<uint16_t>(v) << 1;  // shift in unsigned
	const auto b = static_cast<uint16_t>(v >> 15); // ! must shift in signed
	return  a ^ b;
}

inline uint32_t my_zigzag_encode(int32_t  v) {
	const auto a = static_cast<uint32_t>(v) << 1;  // shift in unsigned
	const auto b = static_cast<uint32_t>(v >> 31); // ! must shift in signed
	return  a ^ b;
}

inline uint64_t my_zigzag_encode(int64_t  v) {
	const auto a = static_cast<uint64_t>(v) << 1;  // shift in unsigned
	const auto b = static_cast<uint64_t>(v >> 63); // ! must shift in signed
	return  a ^ b;
}

inline int8_t   my_zigzag_decode(uint8_t  v) { return static_cast<int8_t >(v >> 1) ^ -static_cast<int8_t >(v & 1); }
inline int16_t  my_zigzag_decode(uint16_t v) { return static_cast<int16_t>(v >> 1) ^ -static_cast<int16_t>(v & 1); }
inline int32_t  my_zigzag_decode(uint32_t v) { return static_cast<int32_t>(v >> 1) ^ -static_cast<int32_t>(v & 1); }
inline int64_t  my_zigzag_decode(uint64_t v) { return static_cast<int64_t>(v >> 1) ^ -static_cast<int64_t>(v & 1); }


#if 1 // byte swap needed

inline uint8_t  my_hton(uint8_t  v) { return my_byte_swap(v); }
inline uint16_t my_hton(uint16_t v) { return my_byte_swap(v); }
inline uint32_t my_hton(uint32_t v) { return my_byte_swap(v); }
inline uint64_t my_hton(uint64_t v) { return my_byte_swap(v); }

inline int8_t   my_hton(int8_t   v) { return (int8_t )my_byte_swap((uint8_t )v); }
inline int16_t  my_hton(int16_t  v) { return (int16_t)my_byte_swap((uint16_t)v); }
inline int32_t  my_hton(int32_t  v) { return (int32_t)my_byte_swap((uint32_t)v); }
inline int64_t  my_hton(int64_t  v) { return (int64_t)my_byte_swap((uint64_t)v); }

inline uint8_t  my_ntoh(uint8_t  v) { return my_byte_swap(v); }
inline uint16_t my_ntoh(uint16_t v) { return my_byte_swap(v); }
inline uint32_t my_ntoh(uint32_t v) { return my_byte_swap(v); }
inline uint64_t my_ntoh(uint64_t v) { return my_byte_swap(v); }

inline int8_t   my_ntoh(int8_t   v) { return (int8_t )my_byte_swap((uint8_t )v); }
inline int16_t  my_ntoh(int16_t  v) { return (int16_t)my_byte_swap((uint16_t)v); }
inline int32_t  my_ntoh(int32_t  v) { return (int32_t)my_byte_swap((uint32_t)v); }
inline int64_t  my_ntoh(int64_t  v) { return (int64_t)my_byte_swap((uint64_t)v); }

#else // byte swap not needed

inline uint8_t  my_hton(uint8_t  v) { return v; }
inline uint16_t my_hton(uint16_t v) { return v; }
inline uint32_t my_hton(uint32_t v) { return v; }
inline uint64_t my_hton(uint64_t v) { return v; }

inline int8_t   my_hton(int8_t   v) { return v; }
inline int16_t  my_hton(int16_t  v) { return v; }
inline int32_t  my_hton(int32_t  v) { return v; }
inline int64_t  my_hton(int64_t  v) { return v; }

inline uint8_t  my_ntoh(uint8_t  v) { return v; }
inline uint16_t my_ntoh(uint16_t v) { return v; }
inline uint32_t my_ntoh(uint32_t v) { return v; }
inline uint64_t my_ntoh(uint64_t v) { return v; }

inline int8_t   my_ntoh(int8_t   v) { return v; }
inline int16_t  my_ntoh(int16_t  v) { return v; }
inline int32_t  my_ntoh(int32_t  v) { return v; }
inline int64_t  my_ntoh(int64_t  v) { return v; }

#endif

inline float  my_hton(float  v) { auto t = my_hton(*(uint32_t*)&v); return *(float* )&t; }
inline double my_hton(double v) { auto t = my_hton(*(uint64_t*)&v); return *(double*)&t; }

inline float  my_ntoh(float  v) { auto t = my_ntoh(*(uint32_t*)&v); return *(float* )&t; }
inline double my_ntoh(double v) { auto t = my_ntoh(*(uint64_t*)&v); return *(double*)&t; }


class MySerializer;
class MyDeserializer;

template<typename SE, typename T>
void my_serializer_io(SE& se, T& v);

class MySerializer {
public:
	MySerializer(std::vector<char>& buf)
		: _buf(buf)
	{
	}

	void io(int8_t   v) { io_var(v); }
	void io(int16_t  v) { io_var(v); }
	void io(int32_t  v) { io_var(v); }
	void io(int64_t  v) { io_var(v); }

	void io(uint8_t  v) { io_var(v); }
	void io(uint16_t v) { io_var(v); }
	void io(uint32_t v) { io_var(v); }
	void io(uint64_t v) { io_var(v); }

	void io(float    v) { _io_fixed_number(v); }
	void io(double   v) { _io_fixed_number(v); }

	template<typename T>
	void io(T& v) {
		my_serializer_io(*this, v);
	}
	
	void io_raw(const void* p, size_t n) {
		const auto s = _buf.size();
		_buf.resize(s + n);
		memcpy(&_buf[s], p, n);
	}

//-----------------------
	void io_fixed(int8_t   v) { _io_fixed_number(v); }
	void io_fixed(int16_t  v) { _io_fixed_number(v); }
	void io_fixed(int32_t  v) { _io_fixed_number(v); }
	void io_fixed(int64_t  v) { _io_fixed_number(v); }

	void io_fixed(uint8_t  v) { _io_fixed_number(v); }
	void io_fixed(uint16_t v) { _io_fixed_number(v); }
	void io_fixed(uint32_t v) { _io_fixed_number(v); }
	void io_fixed(uint64_t v) { _io_fixed_number(v); }

	void io_var(uint8_t  v) {
		if (v >>  7) return _io_var<uint8_t, 2>(v);
					 return _io_var<uint8_t, 1>(v);
	}
	void io_var(uint16_t v) {
		if (v >> 14) return _io_var<uint16_t, 3>(v);
		if (v >>  7) return _io_var<uint16_t, 2>(v);
					 return _io_var<uint16_t, 1>(v);
	}
	void io_var(uint32_t v) {
		if (v >> 28) return _io_var<uint32_t, 5>(v);
		if (v >> 21) return _io_var<uint32_t, 4>(v);
		if (v >> 14) return _io_var<uint32_t, 3>(v);
		if (v >>  7) return _io_var<uint32_t, 2>(v);
					 return _io_var<uint32_t, 1>(v);
	}
	void io_var(uint64_t v) {
		if (v >> 63) return _io_var<uint64_t,10>(v);
		if (v >> 56) return _io_var<uint64_t, 9>(v);
		if (v >> 49) return _io_var<uint64_t, 8>(v);
		if (v >> 42) return _io_var<uint64_t, 7>(v);
		if (v >> 35) return _io_var<uint64_t, 6>(v);
		if (v >> 28) return _io_var<uint64_t, 5>(v);
		if (v >> 21) return _io_var<uint64_t, 4>(v);
		if (v >> 14) return _io_var<uint64_t, 3>(v);
		if (v >>  7) return _io_var<uint64_t, 2>(v);
					 return _io_var<uint64_t, 1>(v);
	}

	void io_var(int8_t   v) { io_var(my_zigzag_encode(v)); }
	void io_var(int16_t  v) { io_var(my_zigzag_encode(v)); }
	void io_var(int32_t  v) { io_var(my_zigzag_encode(v)); }
	void io_var(int64_t  v) { io_var(my_zigzag_encode(v)); }

//-----------------------

	std::vector<char>& buf() { return _buf; }

private:
	template<typename T>
	void _io_fixed_number(T& v) {
		const auto s = _buf.size();
		const auto n = sizeof(v);
		_buf.resize(s + n);
		*reinterpret_cast<T*>(&_buf[s]) = my_hton(v);
	}

	template<typename T, size_t N> // !! using Template size_t N, so compiler might unroll for loop
	void _io_var(T& value) {
		auto s = _buf.size();
		_buf.resize(s + N);
		auto* dst = &_buf[s];
		auto* end = dst + N;
		auto v = value;

		for (size_t i = 0; i < N; i++, dst++) {
			*dst = v & 0x7F;
			v >>= 7;
			if (!v)
				break;
			*dst |= 0x80;
		}
		assert(dst+1 == end);
	}

	std::vector<char>& _buf;
};


class MyDeserializer {
public:
	MyDeserializer(const void* p, size_t n) {
		_r = _start = reinterpret_cast<const char*>(p);
		_end = _r + n;
	}

	MyDeserializer(const std::vector<char>& buf) 
		: MyDeserializer(buf.data(), buf.size() )
	{}

	void io(int8_t   & v) { io_var(v); }
	void io(int16_t  & v) { io_var(v); }
	void io(int32_t  & v) { io_var(v); }
	void io(int64_t  & v) { io_var(v); }
	void io(uint8_t  & v) { io_var(v); }
	void io(uint16_t & v) { io_var(v); }
	void io(uint32_t & v) { io_var(v); }
	void io(uint64_t & v) { io_var(v); }

	void io(float    & v) { _io_fixed_number(v); }
	void io(double   & v) { _io_fixed_number(v); }

	void io_raw(void* dst, size_t n) {
		if (_r + n > _end)
			throw MyError("unexpected end");
		memcpy(dst, _r, n);
		_r += n;
	}

	template<typename T>
	void io(T& v) {
		my_serializer_io(*this, v);
	}

//-----------------------
	void io_fixed(int8_t   & v) { _io_fixed_number(v); }
	void io_fixed(int16_t  & v) { _io_fixed_number(v); }
	void io_fixed(int32_t  & v) { _io_fixed_number(v); }
	void io_fixed(int64_t  & v) { _io_fixed_number(v); }

	void io_fixed(uint8_t  & v) { _io_fixed_number(v); }
	void io_fixed(uint16_t & v) { _io_fixed_number(v); }
	void io_fixed(uint32_t & v) { _io_fixed_number(v); }
	void io_fixed(uint64_t & v) { _io_fixed_number(v); }

	void io_var(uint8_t  & v) { _io_var(v); }
	void io_var(uint16_t & v) { _io_var(v); }
	void io_var(uint32_t & v) { _io_var(v); }
	void io_var(uint64_t & v) { _io_var(v); }

	void io_var(int8_t   & v) { uint8_t  t; _io_var(t); v = my_zigzag_decode(t); }
	void io_var(int16_t  & v) { uint16_t t; _io_var(t); v = my_zigzag_decode(t); }
	void io_var(int32_t  & v) { uint32_t t; _io_var(t); v = my_zigzag_decode(t); }
	void io_var(int64_t  & v) { uint64_t t; _io_var(t); v = my_zigzag_decode(t); }

private:
	template<typename T>
	void _io_fixed_number(T& v) {
		const auto n = sizeof(v);
		if (_r + n > _end)
			throw MyError("unexpected end");
		v = my_ntoh(*reinterpret_cast<const T*>(_r));
		_r += n;
	}

	template<typename T>
	void _io_var(T& value) {
		T v = 0;
		size_t bit = 0;
		for (size_t i = 0;  ; i++) {
			if (_r >= _end)
				throw MyError("MyDeserializer::io_var - unexpected end");

			if (bit >= sizeof(value) * 8)
				throw MyError("MyDeserializer::io_var - excess number limit");

			auto t = *_r;
			_r++;

			v |= (t & 0x7F) << bit;
			if ((t & 0x80) == 0)
				break;
			bit += 7;
		}
		value = v;
	}

	const char* _start;
	const char* _r;
	const char* _end;
};

//-------------------------------
template<typename SE, typename T> inline 
void my_serializer_io(SE& se, T& v) {
	v.io(se);
}

template<> inline
void my_serializer_io(MySerializer& se, std::string& v) {
	uint64_t n = v.size();
	se.io_var(n);
	se.io_raw(v.c_str(), v.size());
}

template<> inline
void my_serializer_io(MyDeserializer& se, std::string& v) {
	uint64_t n64;
	se.io_var(n64);
	if (n64 > std::numeric_limits<size_t>::max())
		throw MyError("string is too big");
	auto n = static_cast<size_t>(n64);
	v.resize(n);
	se.io_raw(&*v.begin(), n);
}

template<typename T> inline
void my_serializer_io(MySerializer& se, std::vector<T>& v) {
	uint64_t n = v.size();
	se.io_var(n);
	for (auto& e : v) {
		se.io(e);
	}
}

template<typename T> inline
void my_serializer_io(MyDeserializer& se, std::vector<T>& v) {
	uint64_t n64;
	se.io_var(n64);
	if (n64 > std::numeric_limits<size_t>::max())
		throw MyError("array is too big");
	auto n = static_cast<size_t>(n64);
	v.resize(n);
	for (auto& e : v) {
		se.io(e);
	}
}
