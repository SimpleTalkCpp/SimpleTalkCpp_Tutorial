#include "my_common.h"

namespace Utf {
	void encodeUtf8(std::string & o, const char32_t* src, size_t srcLen) {
		auto* p = src;
		auto* e = p + srcLen;
		for (; p < e; p++) {
			auto c = (uint32_t)*p;
			if (c < 0x80) {
				// 1 byte
				o.push_back(static_cast<char>(c));
			} else if (c < 0x800) {
				// 2 bytes
				char t[] = {
					static_cast<char>((c >> 6) & 0x1F | 0xC0),
					static_cast<char>( c       & 0x3F | 0x80)
				};
				o.append(t, t + 2);
			} else if (c < 0x10000) {
				char t[] = {
					static_cast<char>((c >> 12) & 0x0F | 0xE0),
					static_cast<char>((c >>  6) & 0x3F | 0x80),
					static_cast<char>( c        & 0x3F | 0x80)
				};
				o.append(t, t + 3);
			} else {
				char t[] = {
					static_cast<char>((c >> 18) & 0x07 | 0xF0),
					static_cast<char>((c >> 12) & 0x3F | 0x80),
					static_cast<char>((c >>  6) & 0x3F | 0x80),
					static_cast<char>( c        & 0x3F | 0x80)
				};
				o.append(t, t + 4);
			}
		}
	}

	void decodeUtf8(std::u32string & o, const char* src, size_t srcLen) {
		auto* p = src;
		auto* e = p + srcLen;
		while (p < e) {
			auto c = (uint8_t)*p;
			if (c < 0xC0) {
				o.push_back(c & 0x7F);
				p++;
			} else if (c < 0xE0) {
				if (p+2 > e) throw MyError("decodeUtf8");
				auto a = (char32_t)*p; p++;
				auto b = (char32_t)*p; p++;
				char32_t v = ((a & 0x1F) << 6) | (b & 0x3F);
				o.push_back(v);
			} else if (c < 0xF0) {
				if (p+3 > e) throw MyError("decodeUtf8");
				auto a = (char32_t)*p; p++;
				auto b = (char32_t)*p; p++;
				auto c = (char32_t)*p; p++;
				char32_t v = ((a & 0x0F) << 12) | ((b & 0x3F) << 6) | (c & 0x3F);
				o.push_back(v);
			} else {
				if (p+4 > e) throw MyError("decodeUtf8");
				auto a = (char32_t)*p; p++;
				auto b = (char32_t)*p; p++;
				auto c = (char32_t)*p; p++;
				auto d = (char32_t)*p; p++;
				char32_t v = ((a & 0x07) << 18) | ((b & 0x3F) << 12) | ((c & 0x3F) << 6) | (c & 0x3F);
				o.push_back(v);
			}
		}
	}

	void encodeUtf16(std::u16string & o, const char32_t* src, size_t srcLen) {
		auto* p = src;
		auto* e = p + srcLen;
		for (; p < e; p++) {
			auto c = (uint32_t)*p;
			if ((c >= 0 && c < 0xD800) ||  (c >= 0xE000 && c < 0x10000)) {
				// 1 char16
				o.push_back(static_cast<char16_t>(c));
			} else {
				char16_t t[] = {
					((c >> 10) & 0x03FF + 0xD800),
					 (c        & 0x03FF + 0xDC00)
				};
				o.append(t, t+2);
			}
		}
	}

	template<typename T>
	size_t getLen(const T* s) {
		auto* p = s;
		for (;*p; p++) {
		}
		return p - s;
	}

	void decodeUtf8(std::u32string & o, const char* src) {
		decodeUtf8(o, src, getLen(src));
	}

	void encodeUtf8(std::string & o, const char32_t* src) {
		encodeUtf8(o, src, getLen(src));
	}

};
