#pragma once
#if defined(_WIN32)
  #if defined(BENGINE_MEMORY_BUILD)
    #define BENGINE_MEMORY_API __declspec(dllexport)
  #else
    #define BENGINE_MEMORY_API __declspec(dllimport)
  #endif
#else
  #define BENGINE_MEMORY_API
#endif
