#pragma once
#if defined(_WIN32)
  #if defined(BENGINE_FRAMEWORK_BUILD)
    #define BENGINE_FRAMEWORK_API __declspec(dllexport)
  #else
    #define BENGINE_FRAMEWORK_API __declspec(dllimport)
  #endif
#else
  #define BENGINE_FRAMEWORK_API
#endif
