//
// Created by Dhananjay Jhinkwan on 22/01/23.
//
#pragma  once
#include <GL/glew.h>

#define Assert(x) if (!(x)) __builtin_trap()
#define GLCall(x) clearGLError(); \
    x;                         \
    Assert(logGLCall(#x, __FILE__, __LINE__ ))

void clearGLError();

bool logGLCall(const char* function, const char* file, int line );