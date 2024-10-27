//
// Created by Felix on 2024/10/27.
//

#ifndef XPLAYER_GLVIDEOVIEW_H
#define XPLAYER_GLVIDEOVIEW_H


#include "IVideoView.h"
#include "XTexture.h"

class GLVideoView : public IVideoView {
public:
    void SetRender(void *win) override;

    void Render(XData data) override;

protected:
    void *win_ = nullptr;
    XTexture *txt = nullptr;
};


#endif //XPLAYER_GLVIDEOVIEW_H
