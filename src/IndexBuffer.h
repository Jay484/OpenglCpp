//
// Created by Dhananjay Jhinkwan on 23/01/23.
//

#pragma once
class IndexBuffer{
private:
    unsigned int m_rendererID;
    unsigned int m_count;
public:
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();
    void Bind() const;
    void Unbind() const;
    inline unsigned get_count() const;
};
