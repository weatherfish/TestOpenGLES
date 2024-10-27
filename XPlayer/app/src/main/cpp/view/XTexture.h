//
// Created by Felix on 2024/10/27.
//

#ifndef XPLAYER_XTEXTURE_H
#define XPLAYER_XTEXTURE_H


class XTexture {

public:
    virtual bool Init(void *win) = 0;

    static XTexture *Create();

    virtual void Draw(unsigned char *data[], int width, int height) = 0;

};


#endif //XPLAYER_XTEXTURE_H
