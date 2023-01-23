#include <iostream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "GLFW/glfw3.h"
#include "Shader.h"

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    std::cout << "I'm apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        std::cout<<"Cant create window"<<std::endl;
        GLCall(glfwTerminate());
        return -1;
    }

    unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    std::cout << "oepngl shader version: " << major << "." << minor << std::endl;

    /* Make the window's context current */
    GLCall(glfwMakeContextCurrent(window));
    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK){
        std::cout<<"Not ok";
    }
    std::cout<<glGetString(GL_VERSION) <<std::endl;

    float vertices[8] = {
            0.5F, 0.5F,
            -0.5F, 0.5F,
            -0.5F, -0.5F,
            0.5F, -0.5F
    };
    unsigned int indices[6] = {
            0,1,2,
            2, 3, 0
    };

    VertexArray vertexArray;
    VertexBuffer vertexBuffer(vertices, 8 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(2);
    vertexArray.addBuffer(vertexBuffer, layout);
    IndexBuffer indexBuffer(indices, 6);

    Shader shader("../res/shaders/basic.shader");
    shader.bind();
    shader.setUniform4f("u_color", 0.0F,1.0F,0.0F,0.8F);


    vertexArray.unBind();
    vertexBuffer.unbind();
    indexBuffer.unbind();
    shader.unbind();

    float red  = 0.0f;
    float inc = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        shader.bind();
        shader.setUniform4f("u_color", red,1.0F,0.0F,0.8F);
        vertexArray.bind();
        indexBuffer.bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT , nullptr));

        if(red> 1.0F){
            inc = -0.05F;
        }else if(red< 0.0F){
            inc = 0.05F;
        }
        red+=inc;
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    GLCall(glfwTerminate());
    return 0;
}