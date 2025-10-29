#pragma once
#if defined(_WIN32)
  #if defined(BENGINE_RENDERER2D_BUILD)
    #define BENGINE_RENDERER2D_API __declspec(dllexport)
  #else
    #define BENGINE_RENDERER2D_API __declspec(dllimport)
  #endif
#else
  #define BENGINE_RENDERER2D_API
#endif
