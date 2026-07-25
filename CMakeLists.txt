cmake_minimum_required(VERSION 3.24)

project(EmergentWorld LANGUAGES CXX)

add_executable(emergent_world
    src/main.cpp
    src/sim/Sim.cpp
    src/pools/CellPool.cpp
    src/factories/CellFactory.cpp
    src/pools/FoodPool.cpp
    src/factories/FoodFactory.cpp
    src/sim/Interactors.cpp
    src/rendering/Camera.cpp
    src/rendering/Renderer.cpp
)

target_include_directories(emergent_world PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pools
    ${CMAKE_CURRENT_SOURCE_DIR}/src/factories
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sim
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering
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