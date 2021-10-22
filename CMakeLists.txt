cmake_minimum_required(VERSION 2.9)

project(MINECRAFT)


add_executable(MINECRAFT src/main.cpp "include/global.hpp" external_includes/glad/src/glad.c  "include/shader.hpp" "src/shader.cpp" "include/stb_image.h" "include/FastNoise.hpp" "src/vRenderEngine.cpp" "src/player.cpp" "src/vEngine.cpp")

include_directories(include external_includes/glm external_includes/glfw-3.3.4/include external_includes/glad/include)

add_subdirectory(external_includes/glfw-3.3.4)

target_link_libraries(MINECRAFT glfw ${CMAKE_DL_LIBS})

file(COPY shaders DESTINATION .)
file(COPY resources DESTINATION .)
