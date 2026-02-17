set(LIB_NAME BRenderer2D)

add_library(${LIB_NAME} SHARED
    Private/Renderer2D.cpp
    Public/BEngineRenderer2DAPI.h
    Public/Renderer2D.h
)

target_include_directories(${LIB_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(${LIB_NAME}
    PUBLIC
        SDL2::SDL2
)

target_compile_definitions(${LIB_NAME}
    PRIVATE
        BENGINE_RENDERER2D_BUILD
    PUBLIC
        $<$<CONFIG:Debug>:BENGINE_DEBUG>
)