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
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

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
            50.0F, 50.0F, 1.0F, 1.0F,
            -50.0F, 50.0F, 0.0F, 1.0F,
            -50.0F, -50.0F, 0.0F, 0.0F,
            50.0F, -50.0F, 1.0F, 0.0F
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
    glm::mat4 view_matrix = glm::translate(glm::mat4(1.0F),glm::vec3(0,0,0));

    Shader shader("../res/shaders/basic.shader");
    shader.bind();

    vertexArray.unBind();
    vertexBuffer.unbind();
    indexBuffer.unbind();
    shader.unbind();
    Texture texture("../res/textures/bishop.png");
    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    glm::vec3 translationA(100,100,0);
    glm::vec3 translationB(400,100,0);

    float red  = 0.0f;
    float inc = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.clear();
        ImGui_ImplGlfwGL3_NewFrame();


        {
            glm::mat4 model_matrix = glm::translate(glm::mat4(1.0F), translationA);
            glm::mat4 mvp = proj_matrix * view_matrix * model_matrix;
            shader.bind();
            shader.setUniformMat4f("u_mvp", mvp);
            renderer.draw(vertexArray, indexBuffer, shader);
        }
        {
            glm::mat4 model_matrix = glm::translate(glm::mat4(1.0F), translationB);
            glm::mat4 mvp = proj_matrix * view_matrix * model_matrix;
            shader.bind();
            shader.setUniformMat4f("u_mvp", mvp);
            renderer.draw(vertexArray, indexBuffer, shader);
        }
        shader.setUniform1i("u_texture", 0);
        texture.Bind(0);

        if(red> 1.0F){
            inc = -0.05F;
        }else if(red< 0.0F){
            inc = 0.05F;
        }
        red+=inc;

        {
            ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0F);
            ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0F);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    GLCall(glfwTerminate());
    return 0;
}