#pragma once

// std::array <-- boost::array
template<typename T, int N>
class MyArray {
public:

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

	bool inBound(int i) const		{ return i >= 0 && i < N; }

	int size() const			{ return N; }
	int capacity() const		{ return N; }

	      T* begin()       { return m_data; }
	const T* begin() const { return m_data; }

	      T* end()         { return m_data + N; }
	const T* end() const   { return m_data + N; }

private:
	void inBoundCheck(int i) const {
		if (!inBound(i)) {
			throw MyError("OutOfBound");
		}
	}
	T m_data[N];
};