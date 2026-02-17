set(LIB_NAME BApplication)

add_library(${LIB_NAME} SHARED
    Private/App.cpp
    Public/App.h
    Public/BEngineApplicationAPI.h
)

target_include_directories(${LIB_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(${LIB_NAME}
    PUBLIC
        BCore
        BRenderer2D
)

target_compile_definitions(${LIB_NAME}
    PRIVATE
        BENGINE_APPLICATION_BUILD
    PUBLIC
        $<$<CONFIG:Debug>:BENGINE_DEBUG>
)