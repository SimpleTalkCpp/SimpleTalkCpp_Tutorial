#include "../my_common.h"
#include <dlfcn.h>

class DynamicLib {
public:

	~DynamicLib() { unload(); }

	void load(const char* filename, int flags) {
		unload();
		_so = dlopen(filename, flags);
		if (!_so) {
			throw MyError("dlopen error");
		}		
	}

	void unload() {
		if (_so) {
			dlclose(_so);
			_so = nullptr;
		}
	}

	void* getFunc(const char* name) {
		return dlsym(_so, name);
	}

private:
	void* _so = nullptr;
};


int main() {
	DynamicLib my_so;
	my_so.load("../my_so/my_so.so", RTLD_NOW);

	typedef int (*Func)();
	auto func = (Func) my_so.getFunc("test_func");
	if (!func) {
		throw MyError("dlsym error");
	}

	func();
	
	return 0;
}