#pragma once

template<class T>
class MyLockedCV {
public:
	using Lock = std::unique_lock<std::mutex>;

	MyLockedCV(T& data, std::mutex& mutex, std::condition_variable& condvar) 
		: _lock(mutex)
	{
		_data  = &data;
		_condvar = &condvar;
	}

	MyLockedCV(MyLockedCV && r) 
		: _lock(std::move(r._lock))
	{
		_condvar = r._condvar;
		_data  = r._data;
		r._condvar = nullptr;
		r._data  = nullptr;
	}

	T* operator->() { return _data; }

	void wait() {
		if (_condvar) {
			_condvar->wait(_lock);
		}
	}

private:
	T*		_data  = nullptr;
	Lock	_lock;
	std::condition_variable* _condvar = nullptr;
};

template<class T>
class MyCondVarProtected {
public:
	MyLockedCV<T> scopedLock() { return MyLockedCV<T>(_data, _mutex, _condvar); }

	void notify_all() {
		_condvar.notify_all();
	}

private:
	T _data;
	std::mutex _mutex;
	std::condition_variable _condvar;
};
