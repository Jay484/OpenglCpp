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

add_executable(
        Unity src/main.cpp src/Renderer.h src/Renderer.cpp src/VertexBuffer.h src/VertexBuffer.cpp src/IndexBuffer.h
        src/IndexBuffer.cpp src/VertexArray.h src/VertexBufferLayout.h src/VertexArray.cpp src/Shader.cpp src/Shader.h
        src/vender/stb_image/stb_image.h src/vender/stb_image/stb_image.cpp src/Texture.cpp src/Texture.h
        src/vender/imgui/imgui_impl_glfw_gl3.h src/vender/imgui/imgui_impl_glfw_gl3.cpp src/vender/imgui/imconfig.h
        src/vender/imgui/imgui.cpp src/vender/imgui/imgui.h src/vender/imgui/imgui_demo.cpp src/vender/imgui/imgui_draw.cpp
        src/vender/imgui/imgui_internal.h src/vender/imgui/imgui_tables.cpp src/vender/imgui/imgui_widgets.cpp
        src/vender/imgui/imstb_rectpack.h src/vender/imgui/imstb_textedit.h src/vender/imgui/imstb_truetype.h
)
target_link_libraries(Unity  libglfw.3.dylib libGLEW.dylib)
include_directories(src/vender/glm)
include_directories(src/vender/imgui)
