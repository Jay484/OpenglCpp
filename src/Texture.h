//
// Created by Dhananjay Jhinkwan on 24/01/23.
//

#pragma once
#include "Renderer.h"
#include <string>

class Texture {
private:
    unsigned int m_rendererId;
    std::string m_fiePath;
    unsigned char* m_localBuffer;
    int m_width, m_height, m_BPP;

public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0 ) const;
    void UnBind() const;
    inline int getHeight() { return m_height; }
    inline int getWidth() { return m_width; }
};
