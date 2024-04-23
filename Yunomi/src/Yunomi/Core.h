#pragma once
//Macros for DLL import/export on Windows

#ifdef YNM_PLATFORM_WINDOWS
	#ifdef YNM_BUILD_DLL
		#define YNM_API __declspec(dllexport)
	#else
		#define YNM_API __declspec(dllimport)
	#endif
#else
	#error Platform not windows
#endif

#ifdef YNM_ENABLE_ASSERTS
	#define YNM_ASSERT(x, ...) { if(!(x)) { YNM_ERROR("ASSERT FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
	#define YNM_CORE_ASSERT(x, ...) { if(!(x)) { YNM_CORE_ERROR("ASSERT FAILED: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define YNM_ASSERT(x, ...)
	#define YNM_CORE_ASSERT(x, ...)
#endif