#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define Assert(x) if (!(x)) __builtin_trap()
#define GLCall(x) clearGLError(); \
    x;                         \
    Assert(logGLCall(#x, __FILE__, __LINE__ ))

static void clearGLError(){
    while(glGetError() != GLEW_NO_ERROR);
}

static bool logGLCall(const char* function, const char* file, int line ){
    while(GLenum error = glGetError()){
        std::cout<<"[OPEN_GL Error] ("<<error<<"): "<<function<<" "<<file<<" :"<<line<<std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource{
    std::string Vertex;
    std::string Fragment;
};

enum class ShaderType{
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
};

static ShaderProgramSource ParseShader(const std::string &filepath){
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while(getline(stream,line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            }else if(line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
        }else{
            if(type != ShaderType::NONE)
                ss[(int) type]<< line<<"\n";
        }
    }

    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string &source){
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout<<"Failed to compile shader"<<std::endl;
        std::cout<<message<<std::endl;
    }

    return id;
}

static unsigned int CreateShader(const std::string &vertex_shader, const std::string &fragment_shader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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

    if(glewInit() != GLEW_OK){
        std::cout<<"Not ok";
    }
    std::cout<<glGetString(GL_VERSION) <<std::endl;

    GLsizei n = 1;
    GLuint buffer;
    float vertices[8] = {
            0.5F, 0.5F,
            -0.5F, 0.5F,
            -0.5F, -0.5F,
            0.5F, -0.5F
    };

    unsigned int VBO;
    GLCall(glGenBuffers(n, &buffer));
    GLCall(glGenVertexArrays(1, &VBO));
    GLCall(glBindVertexArray(VBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_STATIC_DRAW));

    unsigned int indices[6] = {
            0,1,2,
            2, 3, 0
    };
    unsigned int IBO;
    GLCall(glGenBuffers(n, &buffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices , GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2,   0));

    ShaderProgramSource source = ParseShader("../res/shaders/basic.shader");
//    std::cout<<"Vertex source: \n"<<source.Vertex<<std::endl;
    unsigned program = CreateShader(source.Vertex, source.Fragment);
    GLCall(glUseProgram(program));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

//        glDrawArrays(GL_TRIANGLES, 0, 3);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT , nullptr));

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    GLCall(glfwTerminate());
    return 0;
}