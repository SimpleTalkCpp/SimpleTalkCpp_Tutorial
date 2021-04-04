
#ifdef MYDLL_EXPORTS
	#define MYDLL_EXPORT __declspec(dllexport)
#else
	#define MYDLL_EXPORT __declspec(dllimport)
#endif

extern "C" {
	MYDLL_EXPORT int MyDll_Testing();
}