//
// Created by Felix on 2024/10/27.
//

#ifndef XPLAYER_XEGL_H
#define XPLAYER_XEGL_H


class XEGL {
public:
    virtual bool Init(void *win) = 0;

    static XEGL *Get();

    virtual void Draw() = 0;

protected:
    XEGL() {}
};


#endif //XPLAYER_XEGL_H
