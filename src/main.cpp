#include <iostream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
    window = glfwCreateWindow(960, 540, "Hello World", nullptr, nullptr);
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

    float vertices[] = {
            200.0F, 200.0F, 1.0F, 1.0F,
            100.0F, 200.0F, 0.0F, 1.0F,
            100.0, 100.0F, 0.0F, 0.0F,
            200.0F, 100.0F, 1.0F, 0.0F
    };
    unsigned int indices[6] = {
            0,1,2,
            2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray vertexArray;
    VertexBuffer vertexBuffer(vertices, 4* 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    vertexArray.addBuffer(vertexBuffer, layout);
    IndexBuffer indexBuffer(indices, 6);

    glm::mat4 proj_matrix = glm::ortho(0.0F, 960.0F, 0.0F, 540.0F, -1.0F, 1.0F);
    glm::mat4 view_matrix = glm::translate(glm::mat4(1.0F),glm::vec3(-100,0,0));
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0F),glm::vec3(100,100,0));

    glm::mat4 mvp = proj_matrix * view_matrix * model_matrix;

    Shader shader("../res/shaders/basic.shader");
    shader.bind();
    shader.setUniform4f("u_color", 0.0F,1.0F,0.0F,0.8F);
    shader.setUniformMat4f("u_mvp", mvp);

    vertexArray.unBind();
    vertexBuffer.unbind();
    indexBuffer.unbind();
    shader.unbind();

    Renderer renderer;

    float red  = 0.0f;
    float inc = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.clear();
        shader.bind();
        shader.setUniform4f("u_color", red,1.0F,0.0F,0.8F);
        Texture texture("../res/textures/bishop.png");
        texture.Bind(0);
        shader.setUniform1i("u_texture", 0);

        renderer.draw(vertexArray, indexBuffer, shader);

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