# Axe Game: A Minimalist Dodge Game in C++ with Raylib

Axe Game is a simple, yet engaging, dodge game developed in C++ using the powerful and easy-to-use Raylib library. This project serves as a foundational exercise in game development, focusing on core mechanics such as player input, obstacle movement, and collision detection. Players control a purple circle, aiming to evade a vertically moving red axe for as long as possible.

## Features

-   **Intuitive Player Control**: Move the purple circle (player character) smoothly across the screen using either the `W`, `A`, `S`, `D` keys or the `Arrow Keys` for flexible input.
-   **Dynamic Obstacle Movement**: A red axe (enemy) moves vertically within the game window, demonstrating basic AI behavior and collision response by bouncing off screen edges.
-   **Precise Collision Detection**: Implements accurate collision logic to detect contact between the player and the axe, triggering game-over conditions.
-   **Clear Game State Management**: Transitions to a "Game Over" state immediately upon collision, providing clear feedback to the player.
-   **Basic Game Loop**: Manages frame updates, drawing, and input processing within a simple, robust game loop.

## How to Play

1.  **Start**: Compile and run the game executable.
2.  **Movement**: Use `W`, `A`, `S`, `D` or the `Arrow Keys` to control the purple circle.
3.  **Objective**: Dodge the red axe for as long as you can. The game is a test of reflexes and endurance.
4.  **Game Over**: If the purple circle collides with the red axe, the game ends instantly.
5.  **Exit**: Press the `ESC` key at any time to close the game window.

## Compilation and Execution

This project relies on the Raylib library. Before attempting to compile, ensure that Raylib is properly installed and configured on your system.

### Prerequisites

-   **C++ Compiler**: A modern C++ compiler such as `g++` (GNU C++ Compiler), `Clang`, or MSVC (Microsoft Visual C++). Ensure it supports C++11 or newer standards.
-   **Raylib Library**: Download and install Raylib from its [official website](https://www.raylib.com/). Follow the specific installation instructions for your operating system and preferred development environment.

### Linux / macOS

1.  **Clone the repository**:
    ```bash
    git clone https://github.com/your-username/axe-game.git
    cd axe-game
    ```
2.  **Compile the code using the provided Makefile**:
    ```bash
    make
    ```
    *   The `Makefile` is configured to use `pkg-config` for locating Raylib's headers and libraries. Ensure `pkg-config` is installed and Raylib's `.pc` file is accessible in your system's `PKG_CONFIG_PATH`.
    *   If you encounter linking errors, verify that Raylib is correctly installed and its libraries are discoverable by your compiler (e.g., `-lraylib` flag is properly resolved).
3.  **Run the game**:
    ```bash
    ./axe_game
    ```

### Windows (MSVC - Visual Studio)

1.  **Clone the repository**.
2.  **Set up Raylib in Visual Studio**:
    *   Follow the comprehensive official Raylib guide for configuring a C++ project in Visual Studio. This typically involves setting up `Include Directories` (for headers), `Library Directories` (for `.lib` files), and `Additional Dependencies` in the Linker settings (e.g., `raylib.lib`, `winmm.lib`, `gdi32.lib`).
3.  **Add `axe_game.cpp` to your project**: Include the main source file in your Visual Studio project.
4.  **Build and run**: Use Visual Studio's built-in build and run functionalities (`Ctrl+Shift+B` to build, `F5` to run).

### Windows (MinGW / MSYS2)

1.  **Clone the repository**.
2.  **Compile using the Makefile**: If you are using a Unix-like terminal environment (e.g., Git Bash, MSYS2 MinGW-w64 shell) with MinGW-w64 installed, you can use the `make` command:
    ```bash
    make
    ```
    *   Ensure your `PATH` environment variable includes the MinGW `bin` directory.
3.  **Manual Compilation (Alternative)**: Alternatively, compile directly using `g++`:
    ```bash
    g++ axe_game.cpp -o axe_game -lraylib -lopengl32 -lgdi32 -lwinmm -static
    ```
    *   The `-lopengl32`, `-lgdi32`, and `-lwinmm` flags are crucial for linking necessary Windows API libraries that Raylib depends on. The `-static` flag can create a standalone executable.
4.  **Run the game**:
    ```bash
    ./axe_game
    ```