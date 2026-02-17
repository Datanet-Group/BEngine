# Location: C:/Users/Damien/CLionProjects/BEngine/Game/CMakeLists.txt

project(Game)

add_executable(Game
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/TetrisGame.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public/TetrisGame.h"
)

target_include_directories(Game
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private"
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Public"
        "${CMAKE_SOURCE_DIR}"
)

target_link_libraries(Game
        PRIVATE
        BApplication
        BRenderer2D
        BCore
        SDL2::SDL2
)

if(WIN32)
    target_link_libraries(Game
            PRIVATE
            SDL2::SDL2main
    )

    set_target_properties(Game PROPERTIES
            WIN32_EXECUTABLE TRUE
    )

    add_custom_command(TARGET Game POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Game>
            $<TARGET_FILE_DIR:Game>
            COMMAND_EXPAND_LISTS
    )
endif()
