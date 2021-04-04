
#ifdef MY_SO_EXPORT
	#define MySO_API __attribute__ ((__visibility__("default")))
#else
	#define MySO_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

MySO_API int test_func();

#ifdef __cplusplus
}
#endif
