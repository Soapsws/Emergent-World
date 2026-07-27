cmake_minimum_required(VERSION 3.24)

project(EmergentWorld LANGUAGES CXX)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.9
)

FetchContent_Declare(
    rlImGui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG main
)

FetchContent_MakeAvailable(raylib)
FetchContent_Populate(imgui)
FetchContent_Populate(rlImGui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
)

add_library(rlImGui STATIC
    ${rlimgui_SOURCE_DIR}/rlImgui.cpp
)

target_include_directories(rlImGui PUBLIC
    ${rlimgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
)

target_link_libraries(rlImGui PUBLIC
    raylib
    imgui
)

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
    src/sim/Walls.cpp
)

target_include_directories(emergent_world PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pools
    ${CMAKE_CURRENT_SOURCE_DIR}/src/factories
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sim
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utility
)

# Note how dependencies are propagated from above; see target_link_libraries for rlImGui
target_link_libraries(emergent_world PRIVATE
    rlImGui
)

target_compile_features(emergent_world PRIVATE cxx_std_20)
