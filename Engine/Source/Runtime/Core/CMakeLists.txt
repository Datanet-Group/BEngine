set(LIB_NAME BCore)

add_library(${LIB_NAME} SHARED
    Private/Object.cpp
    Private/Input.cpp
    Public/BEngineCoreAPI.h
    Public/IGame.h
    Public/Input.h
    Public/Time.h
    Public/Object.h
)

target_include_directories(${LIB_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

# SDL2 als Abhängigkeit hinzufügen
target_link_libraries(${LIB_NAME}
    PUBLIC
        SDL2::SDL2
)

target_compile_definitions(${LIB_NAME}
    PRIVATE
        BENGINE_CORE_BUILD
    PUBLIC
        $<$<CONFIG:Debug>:BENGINE_DEBUG>
)