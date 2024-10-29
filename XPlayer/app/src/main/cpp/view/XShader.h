//
// Created by Felix on 2024/10/27.
//

#ifndef XPLAYER_XSHADER_H
#define XPLAYER_XSHADER_H

#include <GLES2/gl2.h>
#include "XTexture.h"

class XShader {

public:
    virtual bool Init(XTextureType type);

    virtual void
    GetTexture(unsigned int index, int width, int height, unsigned char *buf,
               bool isAlpha);//获取材质并映射到内存

    virtual void Draw();

protected:
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    GLuint program = 0;

    GLuint texts[100] = {};

};


#endif //XPLAYER_XSHADER_H
