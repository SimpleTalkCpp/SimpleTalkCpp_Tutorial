#pragma once

#include <cstdlib>
#include <new>
#include <stdio.h>
#include <cassert>

class NonCopyable {
public:
	NonCopyable() {}

private:
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};

class MyError : public std::exception {
public:
	MyError(const char* msg) {
		printf("\nMyError %s\n", msg);
	}
};

//template<typename T> inline
//bool isRawCopyable() {
//	return false;
//}

template<typename T>
struct isRawCopyable {
	static const bool value = std::is_pod<T>::value;
};

template<typename T>
void my_memcpy(T* dst, const T* src, int n) {
	if (n < 0)
		return;
	
	if (n * sizeof(T) < 128) {
		T* end = src + n;
		for (;src < end; src++, dst++) {
			*dst = *src;
		}
		return;
	}
	
	memcpy(dst, src, n * sizeof(T));
}

template<typename T>
class MyIVector : public NonCopyable {
public:
	MyIVector() {}
	MyIVector(MyIVector && r)			{ operator=(std::move(r)); }
	virtual ~MyIVector() { 
		// cannot call any virtual function from MyVector !!!!! 
		// it will call MyIVector virtual function !!!
		assert(m_data == nullptr);
	}

	void operator=(MyIVector && r);

	// TODO
//	MyVector(const MyVector& r)		{ operator=(r); }
//	void operator=(const MyVector& r);

//	bool operator==(const MyVector& r) {
//	}

		// no inBound check
	      T& unsafeAt(int i)       { return m_data[i]; }
	const T& unsafeAt(int i) const { return m_data[i]; }

	      T& at(int i)       { inBoundCheck(i); return m_data[i]; }
	const T& at(int i) const { inBoundCheck(i); return m_data[i]; }

	      T& operator[](int i)       { return at(i); }
	const T& operator[](int i) const { return at(i); }

		  T& back()					{ return at(m_size -1); }
	const T& back() const			{ return at(m_size -1); }

		  T& back(int i)			{ return at(m_size -i -1); }
	const T& back(int i) const		{ return at(m_size -i -1); }

	void clear();
	void clearAndFree();

	bool inBound(int i) const		{ return i >= 0 && i < m_size; }

	void append(const T& v);
	void append(T && v);

	template<typename... ARGS>
	void emplace_back(ARGS&&... args);

	void removeAt(int i); // TODO
	void removeAndSwapLast(); // TODO

	int  size		() const { return m_size; }

	template<typename... ARGS>
	void resize		(int newSize, ARGS&&... args);

	void reserve	(int n);
	int	 capacity	() const { return m_capacity; }

			T* data()				{ return m_data; }
	const	T* data() const			{ return m_data; }

			T* begin()				{ return m_data; }
	const	T* begin() const		{ return m_data; }

			T* end	()				{ return m_data + m_size; }
	const	T* end  () const		{ return m_data + m_size; }

	template<typename A>
	class RevIterator_ {
	public:
		RevIterator_(A* p) : m_p(p) {}

		A& operator*() { return *m_p; }
		bool operator==(const RevIterator_& r) const { return m_p == r.m_p; }
		bool operator!=(const RevIterator_& r) const { return m_p != r.m_p; }

		void operator++() { --m_p; } // <----- reverse order

	private:
		A* m_p;
	};

	using RevIterator  = RevIterator_<T>;
	using CRevIterator = RevIterator_<const T>;

	RevIterator		rbegin()		{ return m_data + m_size - 1; }
	CRevIterator	rbegin() const	{ return m_data + m_size - 1; }

	RevIterator		rend()			{ return m_data - 1; }
	CRevIterator	rend() const	{ return m_data - 1; }

	template<typename A>
	class RevEnumerator_ {
	public:
		RevEnumerator_(RevIterator_<A> begin, RevIterator_<A> end) 
			: m_begin(begin)
			, m_end(end)
		{
		}

		RevIterator_<A> begin()	{ return m_begin; }
		RevIterator_<A> end()	{ return m_end; }

	private:
		RevIterator_<A> m_begin;
		RevIterator_<A> m_end;
	};

	using RevEnumerator  = RevEnumerator_<T>;
	using CRevEnumerator = RevEnumerator_<const T>;

	RevEnumerator  revEach()		{ return RevEnumerator( rbegin(), rend()); }
	CRevEnumerator revEach() const	{ return CRevEnumerator(rbegin(), rend()); }

protected:
	virtual T*		onMalloc(int n) = 0;
	virtual void	onFree(T* p) = 0;
	virtual bool	isUsingLocalBuf() const = 0;

private:
	void inBoundCheck(int i) const {
		if (!inBound(i)) {
			throw MyError("OutOfBound");
		}
	}

	T*	m_data = nullptr;
	int	m_size = 0;
	int	m_capacity = 0;
};

template<typename T, int LOCAL_BUF_SIZE>
class LocalBuf : public NonCopyable {
public:
	      T* localBuf()       { return reinterpret_cast<T*>(_localBuf); }
	const T* localBuf() const { return reinterpret_cast<const T*>(_localBuf); }
private:
	char _localBuf[LOCAL_BUF_SIZE * sizeof(T)];
};

// template specialization for char buf[0]
template<typename T>
class LocalBuf<T, 0> : public NonCopyable {
public:
	      T* localBuf()       { return nullptr; }
	const T* localBuf() const { return nullptr; }
private:
};

template<typename T, int LOCAL_BUF_SIZE = 8>
class MyVector : public MyIVector<T>, public LocalBuf<T, LOCAL_BUF_SIZE> {
	using BUF = LocalBuf<T, LOCAL_BUF_SIZE>;
public:
	virtual ~MyVector() {
		clearAndFree();
	}

protected:
	virtual T*	onMalloc(int n) {
		if (n <= LOCAL_BUF_SIZE) {
			return BUF::localBuf();
		} else {
			return reinterpret_cast<T*>(std::malloc(n * sizeof(T)));
		}
	}

	virtual void onFree(T* p) {
		if (p && p != BUF::localBuf()) {
			std::free(p);
		}
	}

	virtual bool isUsingLocalBuf() const { return localBuf() == data(); }
};

template<typename T>
void MyIVector<T>::clear() {
	auto* dst = m_data;
	auto* end = m_data + m_size;

	for (; dst < end; dst++) {
		dst->~T();
	}
	m_size = 0;
}

template<typename T>
void MyIVector<T>::operator=(MyIVector && r) {
	clearAndFree();

	if (r.isUsingLocalBuf()) {
		reserve(r.m_size);
		auto* dst = m_data;
		auto* src = r.m_data;
		auto* end = r.m_data + r.m_size;

		for (; src < end; dst++, src++) {
			new(dst) T(std::move(*src));
		}
		m_size = r.m_size;
		r.clear();

	}else{
		m_data = r.m_data;
		m_size = r.m_size;
		m_capacity = r.m_capacity;

		r.m_data = nullptr;
		r.m_size = 0;
		r.m_capacity = 0;
	}
}

template<typename T> inline
void MyIVector<T>::clearAndFree() {
	clear();
	onFree(m_data);
	m_data = nullptr;
	m_capacity = 0;
}

template<typename T>
template<typename... ARGS>
void MyIVector<T>::emplace_back(ARGS&&... args) {
	reserve(m_size + 1);
	new (m_data + m_size) T(std::forward<ARGS>(args)...);
	m_size++;
}

template<typename T> inline
void MyIVector<T>::append(const T& v) {
	reserve(m_size + 1);
	new (m_data + m_size) T(v);
	m_size++;
}

template<typename T> inline
void MyIVector<T>::append(T && v) {
	reserve(m_size + 1);
	new (m_data + m_size) T(std::forward<T>(v));
	m_size++;
}

template<typename T> inline
void MyIVector<T>::reserve(int n)
{
	if (m_capacity >= n)
		return;

	auto newCapacity = m_capacity + m_capacity / 2; // try to grow 1.5x
	if (newCapacity < n) {
		newCapacity = n;
	}

	auto* newData = onMalloc(newCapacity);
	if (newData != m_data) {
		// move elements
		try {
			auto* src = m_data;
			auto* end = m_data + m_size;
			auto* dst = newData;

			if (isRawCopyable<T>::value) {
				memcpy(dst, src, sizeof(T) * m_size);
			} else {
				for (; src < end; src++, dst++) {
					new(dst) T(std::move(*src)); //placement new
					src->~T();
				}
			}
		} catch (...) {
			onFree(newData);
			throw;
		}
	}

	m_data = newData;
	m_capacity = newCapacity;
}

template<typename T> 
template<typename... ARGS> inline
void MyIVector<T>::resize(int newSize, ARGS&&... args)
{
	if (m_size == newSize)
		return;
	reserve(newSize);

	if (newSize > m_size) {
		// call ctor
		auto* dst = m_data + m_size;
		auto* end = m_data + newSize;
		for (; dst < end; dst++) {
			new (dst) T(std::forward<ARGS>(args)...);
		}
	} else if (newSize < m_size) {
		// call dtor
		auto* dst = m_data + newSize;
		auto* end = m_data + m_size;
		for (; dst < end; dst++) {
			dst->~T();
		}
	}

	m_size = newSize;
}
