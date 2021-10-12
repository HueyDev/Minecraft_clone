cmake_minimum_required(VERSION 2.9)

project(MINECRAFT)


add_executable(MINECRAFT src/main.cpp "include/chunk.hpp" "include/voxel.hpp" "src/voxel.cpp" "src/chunk.cpp" "include/global.hpp" "src/PerlinNoise.cpp" "include/renderChunk.hpp" "src/renderChunk.cpp" "include/game.hpp" "src/game.cpp" external_includes/glad/src/glad.c "include/camera.hpp" "src/camera.cpp" "include/shader.hpp" "src/shader.cpp" "include/stb_image.h")

include_directories(include external_includes/glm external_includes/glfw-3.3.4/include external_includes/glad/include)

add_subdirectory(external_includes/glfw-3.3.4)

target_link_libraries(MINECRAFT glfw ${CMAKE_DL_LIBS})

file(COPY shaders DESTINATION .)
file(COPY resources DESTINATION .)
