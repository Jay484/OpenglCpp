//
// Created by Dhananjay Jhinkwan on 23/01/23.
//
#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray{
private:
    unsigned int m_rendererId;
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer &vertexBuffer, const VertexBufferLayout &vertexBufferLayout);
    void bind() const;
    void unBind() const;
};
