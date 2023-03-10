//
// Created by Dhananjay Jhinkwan on 23/01/23.
//
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource{
    std::string Vertex;
    std::string Fragment;
};

class Shader {
private:
    std::string m_filePath;
    unsigned int m_rendererId;
    std::unordered_map<std::string, int> m_uniformLocationCache;
public:
    Shader(const std::string& filePath);
    ~Shader();

    void bind() const;
    void unbind() const;

    //set uniforms
    void setUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
    void setUniform1i(const std::string& name, int value);
    void setUniformMat4f(const std::string &name, const glm::mat4& mat);
private:
    int getUniformLocation(const std::string& name);
    unsigned int createShader(const std::string &vertex_shader, const std::string &fragment_shader);
    unsigned int compileShader(unsigned int type, const std::string &source);
    ShaderProgramSource parseShader(const std::string &filepath);
};
