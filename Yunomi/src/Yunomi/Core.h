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