cmake_minimum_required(VERSION 3.24)
project(Unity)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES src/main.cpp)

#Set up OpenGL
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})

#Set up GLEW
include_directories(/usr/local/homebrew/Cellar/glew/2.2.0_1/include)
link_directories(/usr/local/homebrew/lib)

#Set up GLFW
include_directories(Dependencies/glfw-3.3.8.bin.MACOS/include)
link_directories(Dependencies/glfw-3.3.8.bin.MACOS/lib-x86_64/)

link_libraries(${OPENGL_LIBRARIES})
link_libraries(${GLEW_LIBRARIES})

add_executable(Unity src/main.cpp)
target_link_libraries(Unity  libglfw.3.dylib libGLEW.dylib)