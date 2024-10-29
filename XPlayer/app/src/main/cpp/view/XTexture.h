//
// Created by Felix on 2024/10/27.
//

#ifndef XPLAYER_XTEXTURE_H
#define XPLAYER_XTEXTURE_H

enum XTextureType {
    TEXTURE_YUV420P = 0,  //yyyy uv yyyy uv
    TEXTURE_NV12 = 25,    //yyyy yyyy uv uv
    TEXTURE_NV21 = 26,    //yyyy yyyy vu vu
};

class XTexture {

public:
    virtual bool Init(void *win, XTextureType type) = 0;

    static XTexture *Create();

    virtual void Draw(unsigned char *data[], int width, int height) = 0;

};


#endif //XPLAYER_XTEXTURE_H
