cmake_minimum_required(VERSION 3.16)

project(DominoExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------
# Magnum
# -----------------------------
find_package(Magnum REQUIRED
    GL
    MeshTools
    Primitives
    Shaders
    Sdl2Application
    Trade
)

# -----------------------------
# Executable
# -----------------------------
add_executable(domino
    src/main.cpp
)

# -----------------------------
# Bullet (Linux fallback safe)
# -----------------------------
target_include_directories(domino PRIVATE
    /usr/include/bullet
)

# -----------------------------
# Link libraries
# -----------------------------
target_link_libraries(domino PRIVATE
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders
    Magnum::Trade

    BulletDynamics
    BulletCollision
    LinearMath
)

# -----------------------------
# Compiler warnings
# -----------------------------
if(MSVC)
    target_compile_options(domino PRIVATE /W4)
else()
    target_compile_options(domino PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
