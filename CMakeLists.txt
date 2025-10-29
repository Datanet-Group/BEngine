cmake_minimum_required(VERSION 3.20)
project(BEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setze Ausgabeverzeichnisse für alle Targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Debug-Bibliotheken-Suffix
set(CMAKE_DEBUG_POSTFIX "d")

# SDL2 finden
find_package(SDL2 CONFIG REQUIRED)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Füge die Engine-Module hinzu
add_subdirectory(Engine)

# Dann das Game
add_subdirectory(Game)