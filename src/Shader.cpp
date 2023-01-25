//
// Created by Dhananjay Jhinkwan on 23/01/23.
//

#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string &filePath)
    : m_filePath(filePath), m_rendererId(0)
{
    ShaderProgramSource source = parseShader(filePath);
//    std::cout<<"Vertex source: \n"<<source.Vertex<<std::endl;
//    std::cout<<"Fragment source: \n"<<source.Fragment<<std::endl;
    m_rendererId = createShader(source.Vertex, source.Fragment);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_rendererId));
}

void Shader::bind() const {
    GLCall(glUseProgram(m_rendererId));
}

void Shader::unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::setUniform4f(const std::string &name, float f0, float f1, float f2, float f3) {
    GLCall(glUniform4f(getUniformLocation(name), f0, f1, f2, f3));
}

void Shader::setUniform1i(const std::string &name, int value) {
    GLCall(glUniform1i(getUniformLocation(name),value));
}

void Shader::setUniformMat4f(const std::string &name, const glm::mat4& mat) {
    GLCall(glUniformMatrix4fv(getUniformLocation(name),1, GL_FALSE, &mat[0][0]));
}

enum class ShaderType{
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
};

ShaderProgramSource Shader::parseShader(const std::string &filepath){
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

unsigned int Shader::compileShader(unsigned int type, const std::string &source){
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

unsigned int Shader::createShader(const std::string &vertex_shader, const std::string &fragment_shader){
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int Shader::getUniformLocation(const std::string &name) {
    if(m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];
    GLCall(int location = glGetUniformLocation(m_rendererId, name.c_str()));
    if(location == -1){
        std::cout<< "Warning: uniform '"<<name<<"' doesn't exist!!"<<std::endl;
    }
    m_uniformLocationCache[name] = location;
    return location;
}
