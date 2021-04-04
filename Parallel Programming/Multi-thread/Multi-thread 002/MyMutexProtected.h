#pragma once

template<class T>
class MyLocked {
public:
	using Lock = std::unique_lock<std::mutex>;

	MyLocked(T& data, std::mutex& mutex) 
		: _lock(mutex)
	{
		_data  = &data;
	}

	MyLocked(MyLocked && r)
		: _lock(std::move(r._lock))
	{
		_data  = r._data;
		r._data  = nullptr;
	}

	T* operator->() { return _data; }
private:
	T*	_data  = nullptr;
	Lock _lock;
};

template<class T>
class MyMutexProtected {
public:
	MyLocked<T> scopedLock() { return MyLocked<T>(_data, _mutex); }
private:
	T _data;
	std::mutex _mutex;
};