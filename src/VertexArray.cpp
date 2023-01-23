//
// Created by Dhananjay Jhinkwan on 23/01/23.
//

#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &m_rendererId));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_rendererId));
}

void VertexArray::addBuffer(const VertexBuffer &vertexBuffer, const VertexBufferLayout &vertexBufferLayout) {
    bind();
    vertexBuffer.Bind();
    const auto& elements = vertexBufferLayout.getElements();
    unsigned int offset = 0;
    for(unsigned int i = 0 ; i< elements.size(); i++){
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(
                i,
                element.count,
                element.type,
                element.normalized ,
                vertexBufferLayout.getStride(),
                (const void*) offset
        ));
        offset+= element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const {
    GLCall(glBindVertexArray(m_rendererId));
}

void VertexArray::unBind() const {
    GLCall(glBindVertexArray(0));
}
