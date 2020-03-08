cmake_minimum_required(VERSION 3.14)
project(Cherno_OpenGL)

set(CMAKE_CXX_STANDARD 14)

add_executable(Cherno_OpenGL main.cpp Renderer.cpp Renderer.h VertexBuffer.cpp VertexBuffer.h IndexBuffer.cpp IndexBuffer.h)

# OpenGL
find_package(glfw3 3.1.2 REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# GLEW
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} GLEW)