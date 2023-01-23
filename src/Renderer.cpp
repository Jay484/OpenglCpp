//
// Created by Dhananjay Jhinkwan on 22/01/23.
//
#include "Renderer.h"
#include <iostream>

void clearGLError(){
    while(glGetError() != GLEW_NO_ERROR);
}

bool logGLCall(const char* function, const char* file, int line ){
    while(GLenum error = glGetError()){
        std::cout<<"[OPEN_GL Error] ("<<error<<"): "<<function<<" "<<file<<" :"<<line<<std::endl;
        return false;
    }
    return true;
}