#pragma

#ifdef __linux__

#ifdef _EXPORT
#   define API __attribute__((visibility("default")))
#else
#   define API
#endif // !

#endif // !__linux__
