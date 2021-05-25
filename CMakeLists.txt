cmake_minimum_required(VERSION 3.19)
project(Renderer3d)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/SFML)

include_directories(lib/eigen)
include_directories(Structures)

add_executable(Renderer3d main.cpp
        Structures/Primitive.cpp
        Structures/Screen.cpp
        Structures/Renderer.cpp
        Structures/Scene.cpp
        Structures/Camera.cpp)

target_link_libraries(Renderer3d sfml-graphics)