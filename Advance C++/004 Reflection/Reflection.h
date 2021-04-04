#pragma once

#include <stdint.h>
#include <type_traits>
#include <map>
#include <string>

#define MY_TYPE(T, BASE) \
private: \
	using This = T; \
	using Base = BASE; \
	class TI_Base : public TypeInfoInit<T, BASE> { \
	public: \
		TI_Base() : TypeInfoInit<T, BASE>(#T) {} \
	}; \
public: \
	static const TypeInfo* s_getType(); \
	virtual const TypeInfo* getType() const override { return s_getType(); } \
private: \
//----

class MyObject;
class TypeInfo;
template<class T> const TypeInfo* my_typeof();

class TypeManager {
public:

	static TypeManager* instance();

	const TypeInfo* find(const char* name) {
		auto it = m_map.find(name);
		if (it == m_map.end()) return nullptr;
		return it->second;
	}

	void registerType(const char* name, const TypeInfo* ti) {
		m_map[name] = ti;
	}

private:
	std::map<std::string, const TypeInfo*> m_map;
};

template<class OBJ, class FIELD> inline
intptr_t my_offset(FIELD OBJ::*ptr) {
	OBJ* obj = nullptr;
	auto* m = &(obj->*ptr);
	return reinterpret_cast<intptr_t>(m);
}

class FieldInfo {
public:

	template<class OBJ, class FIELD>
	FieldInfo(const char* name_, FIELD OBJ::*ptr)
		: name(name_)
		, fieldType(my_typeof<FIELD>())
		, offset(my_offset(ptr))
	{
	}

	const char* name = "";
	const TypeInfo* fieldType = nullptr;
	intptr_t offset = 0;
};

class FieldsEnumerator {
public:
	FieldsEnumerator(const TypeInfo* typeInfo_) 
		: typeInfo(typeInfo_) 
	{}

	class Iterator {
	public:
		Iterator(const TypeInfo* typeInfo_, size_t fieldIndex_)
			: typeInfo(typeInfo_)
			, fieldIndex(fieldIndex_)
		{
		}

		bool operator!=(const Iterator& r) const {
			return typeInfo != r.typeInfo || fieldIndex != r.fieldIndex;
		}

		void operator++();

		const FieldInfo& operator*();

	private:
		const TypeInfo* typeInfo = nullptr;
		size_t fieldIndex = 0;
	};

	Iterator begin() { return Iterator(typeInfo, 0); }
	Iterator end() { return Iterator(nullptr, 0); }

	const TypeInfo* typeInfo = nullptr;
};


class TypeInfo {
public:
	const char* name = "";
	const TypeInfo* base = nullptr;

	size_t dataSize = 0;

	const FieldInfo* fieldsArray = nullptr;
	size_t fieldCount = 0;

	FieldsEnumerator fields() const { return FieldsEnumerator(this); }

	typedef MyObject* (*Creator)();

	Creator creator;

	bool isContainer = false;
	const TypeInfo* elementType = nullptr;

	MyObject* createObject() const {
		if (!creator) return nullptr;
		return creator();
	}

	bool isKindOf(const TypeInfo* t) const {
		auto* p = this;
		while (p) {
			if (p == t) return true;
			p = p->base;
		}
		return false;
	}

	template<class R>
	bool isKindOf() const {
		return isKindOf(my_typeof<R>());
	}
};

class MyObject;
template<> const TypeInfo* my_typeof<MyObject>();

class MyObject {
public:
	virtual const TypeInfo* getType() const {
		return my_typeof<MyObject>();
	}

	float x = 0;
	float y = 0;
};

inline const TypeInfo* my_typeof(MyObject& obj) {
	return obj.getType();
}

template<class DST> inline
DST* my_cast(MyObject* src) {
	if (!src) return nullptr;
	auto* ti = my_typeof(*src);
	if (!ti) return nullptr;
	if (!ti->isKindOf<DST>()) return nullptr;
	return static_cast<DST*>(src);
}

template<> const TypeInfo* my_typeof<float>();

inline
void FieldsEnumerator::Iterator::operator++() {
	if (!typeInfo) return;

	fieldIndex++;
	for (;;) {
		if (!typeInfo) return;
		if (fieldIndex < typeInfo->fieldCount) return;

		typeInfo = typeInfo->base;
		fieldIndex = 0;
	}
}

inline
const FieldInfo& FieldsEnumerator::Iterator::operator*() {
	return typeInfo->fieldsArray[fieldIndex];
}

template<class T>
class TypeInfoInitNoBase : public TypeInfo {
public:
	TypeInfoInitNoBase(const char* name_) {
		name = name_;
		dataSize = sizeof(T);
	};

	template<size_t N>
	void setFields(const FieldInfo (&fi)[N]) {
		fieldsArray = fi;
		fieldCount = N;
	}
};

template<class T> inline
static MyObject* TypeCreator() {
	return new T();
}

template<class T, class Base>
class TypeInfoInit : public TypeInfoInitNoBase<T> {
public:
	TypeInfoInit(const char* name_)
		: TypeInfoInitNoBase<T>(name_)
	{
		static_assert(std::is_base_of<Base, T>::value, "invalid base class");
		this->base = my_typeof<Base>();

		this->creator = &TypeCreator<T>;
	};
};

#define my_typeof_define(T) \
	template<> const TypeInfo* my_typeof<T>();
//----

my_typeof_define(float)
my_typeof_define(double)

my_typeof_define(int8_t)
my_typeof_define(int16_t)
my_typeof_define(int32_t)
my_typeof_define(int64_t)

my_typeof_define(uint8_t)
my_typeof_define(uint16_t)
my_typeof_define(uint32_t)
my_typeof_define(uint64_t)

// my_typeof_define(char8_t) c++20
my_typeof_define(char16_t)
my_typeof_define(char32_t)
my_typeof_define(wchar_t)

template<class T> inline
const TypeInfo* my_typeof() {
	return T::s_getType();
}
