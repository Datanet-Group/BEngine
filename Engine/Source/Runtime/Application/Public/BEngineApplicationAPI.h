#pragma once
#if defined(_WIN32)
  #if defined(BENGINE_APPLICATION_BUILD)
    #define BENGINE_APPLICATION_API __declspec(dllexport)
  #else
    #define BENGINE_APPLICATION_API __declspec(dllimport)
  #endif
#else
  #define BENGINE_APPLICATION_API
#endif
