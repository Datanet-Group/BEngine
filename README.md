# BEngine - 2D Game Engine

A simple 2D game engine built with C++20 and SDL2. Includes a Tetris game demonstration.

## Features

- Cross-platform support (Windows, Linux)
- SDL2-based rendering
- Input handling system
- Game loop with fixed timestep
- Modular architecture with separate runtime modules

## Dependencies

- **CMake** 3.20 or higher
- **C++20** compatible compiler (GCC 13+, MSVC 19.30+, Clang 14+)
- **SDL2** development libraries

### Installing Dependencies

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y cmake build-essential libsdl2-dev
```

#### Windows
1. Install CMake from https://cmake.org/download/
2. Install Visual Studio with C++ support
3. Install SDL2 development libraries using vcpkg:
   ```
   vcpkg install sdl2:x64-windows
   ```

#### macOS
```bash
brew install cmake sdl2
```

## Building

### Linux/macOS
```bash
mkdir build
cd build
cmake ..
cmake --build . -j$(nproc)
```

### Windows (Visual Studio)
```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

## Running

After building, the executable will be located at:
- Linux/macOS: `build/bin/Game`
- Windows: `build/bin/Release/Game.exe` or `build/bin/Debug/Game.exe`

### Tetris Controls
- **Arrow Left/Right**: Move piece horizontally
- **Z**: Rotate counter-clockwise
- **X**: Rotate clockwise
- **Down Arrow**: Soft drop (faster falling)
- **Space**: Hard drop (instant drop)
- **R**: Reset game (when game over)
- **ESC** or close window: Quit

## Project Structure

```
BEngine/
├── Engine/                    # Core engine modules
│   └── Source/
│       └── Runtime/
│           ├── Application/   # Application and window management
│           ├── Core/          # Core utilities (Input, Time, etc.)
│           └── Renderer2D/    # 2D rendering system
├── Game/                      # Game implementation (Tetris)
│   └── Source/
│       ├── Private/           # Implementation files
│       └── Public/            # Header files
└── CMakeLists.txt            # Root build configuration
```

## Architecture

The engine is structured as three main modules:

1. **BCore**: Core functionality (input handling, time management, game interface)
2. **BRenderer2D**: 2D rendering capabilities using SDL2
3. **BApplication**: Application lifecycle and window management

All modules are built as shared libraries and linked into the game executable.

## License

[Add your license information here]
