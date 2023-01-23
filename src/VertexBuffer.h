//
// Created by Dhananjay Jhinkwan on 22/01/23.
//

#pragma once
class VertexBuffer{
private:
    unsigned int m_rendererID;
public:
    VertexBuffer(const void *data, unsigned int size);
    ~VertexBuffer();
    void Bind() const;
    void Unbind() const;
};
