cmake_minimum_required(VERSION 3.24)

project(EmergentWorld LANGUAGES CXX)

add_executable(emergent_world
    src/main.cpp
    src/Sim.cpp
    src/CellPool.cpp
    src/CellFactory.cpp
    src/FoodPool.cpp
    src/FoodFactory.cpp
    src/Interactors.cpp
)

target_link_libraries(emergent_world PRIVATE raylib)

target_compile_features(emergent_world PRIVATE cxx_std_20)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
)

FetchContent_MakeAvailable(raylib)