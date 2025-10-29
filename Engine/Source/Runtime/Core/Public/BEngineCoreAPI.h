#pragma once
#if defined(_WIN32)
  #if defined(BENGINE_CORE_BUILD)
    #define BENGINE_CORE_API __declspec(dllexport)
  #else
    #define BENGINE_CORE_API __declspec(dllimport)
  #endif
#else
  #define BENGINE_CORE_API
#endif
