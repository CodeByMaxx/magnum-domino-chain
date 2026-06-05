cmake_minimum_required(VERSION 3.16)

project(DominoExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Magnum REQUIRED
    GL
    MeshTools
    Primitives
    Shaders
    Sdl2Application
)

find_package(Bullet REQUIRED)

message(STATUS "Bullet include dirs: ${BULLET_INCLUDE_DIRS}")
message(STATUS "Bullet libraries: ${BULLET_LIBRARIES}")

add_executable(domino
    ./src/main.cpp
)

target_include_directories(domino PRIVATE
    /usr/include/bullet
)

target_link_libraries(domino PRIVATE
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders

    BulletDynamics
    BulletCollision
    LinearMath
)

message(STATUS "Magnum version: ${Magnum_VERSION}")

if(MSVC)
    target_compile_options(domino PRIVATE /W4)
else()
    target_compile_options(domino PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
