#ifdef GMKEYBOARD_EXPORTS
#define GMKEYBOARD_API __declspec(dllexport)
#else
#define GMKEYBOARD_API __declspec(dllimport)
#endif