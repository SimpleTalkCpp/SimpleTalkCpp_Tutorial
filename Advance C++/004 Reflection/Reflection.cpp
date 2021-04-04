#include "Reflection.h"

#define my_typeof_impl(T) \
	template<> const TypeInfo* my_typeof<T>() { \
		static TypeInfoInitNoBase<T> ti(#T); \
		return &ti; \
	} \
//----

my_typeof_impl(float)
my_typeof_impl(double)

my_typeof_impl(int8_t)
my_typeof_impl(int16_t)
my_typeof_impl(int32_t)
my_typeof_impl(int64_t)

my_typeof_impl(uint8_t)
my_typeof_impl(uint16_t)
my_typeof_impl(uint32_t)
my_typeof_impl(uint64_t)

// my_typeof_define(char8_t) c++20
my_typeof_impl(char16_t)
my_typeof_impl(char32_t)
my_typeof_impl(wchar_t)

template<> const TypeInfo* my_typeof<MyObject>() {
	class TI : public TypeInfo {
	public:
		TI() {
			name = "MyObject";
		}
	};
	static TI ti;
	return &ti;
}

TypeManager* TypeManager::instance() {
	static TypeManager m;
	return &m;
}

