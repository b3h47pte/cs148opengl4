#pragma once

#ifndef __TEXTURE__
#define __TEXTURE__

#include "common/common.h"

class Texture: public std::enable_shared_from_this<Texture>
{
public:
    Texture(GLubyte* rawData, int width, int height);
    virtual ~Texture();
    
    virtual void BeginRender(int unit) const;
    virtual void EndRender() const;
private:
    int texWidth;
    int texHeight;

    GLuint glTexture;
};

#endif
