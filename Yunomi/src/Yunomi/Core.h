#pragma once

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #define YNM_PLATFORM_WINDOWS
#elif defined(__linux__)
    #define YNM_PLATFORM_LINUX
#else
    #error "Unsupported platform"
#endif

// DLL import/export macros
#ifdef YNM_PLATFORM_WINDOWS
    #ifdef YNM_BUILD_DLL
        #define YNM_API __declspec(dllexport)
    #else
        #define YNM_API __declspec(dllimport)
    #endif
#elif defined(YNM_PLATFORM_LINUX)
    #ifdef YNM_BUILD_DLL
        #define YNM_API __attribute__((visibility("default")))
    #else
        #define YNM_API
    #endif
#else
    #define YNM_API
#endif

// Assertion macros
#ifdef YNM_ENABLE_ASSERTS
    #define YNM_ASSERT(x, ...) { if(!(x)) { YNM_ERROR("ASSERT FAILED: {0}", __VA_ARGS__); __builtin_trap(); } }
    #define YNM_CORE_ASSERT(x, ...) { if(!(x)) { YNM_CORE_ERROR("ASSERT FAILED: {0}", __VA_ARGS__); __builtin_trap(); } }
#else
    #define YNM_ASSERT(x, ...)
    #define YNM_CORE_ASSERT(x, ...)
#endif
