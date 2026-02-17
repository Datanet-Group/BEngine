set(LIB_NAME BMemory)

add_library(${LIB_NAME} SHARED
        Private/Memory.cpp
        Private/DefaultAllocator.cpp
        Private/FrameArena.cpp
        Private/NewDelete.cpp
        Public/Memory.h
        Public/Allocator.h
        Public/DefaultAllocator.h
        Public/BEngineMemoryAPI.h
)

target_include_directories(${LIB_NAME}
        PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Public
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

# Export from this DLL + enable tracking in Debug
target_compile_definitions(${LIB_NAME}
        PRIVATE BENGINE_MEMORY_BUILD
        PUBLIC  $<$<CONFIG:Debug>:BENGINE_MEM_TRACK=1>
)

# Keep it standalone (no links back to other engine DLLs)
target_link_libraries(${LIB_NAME}
        PUBLIC
)
