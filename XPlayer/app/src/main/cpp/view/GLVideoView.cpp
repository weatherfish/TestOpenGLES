//
// Created by Felix on 2024/10/27.
//

#include "GLVideoView.h"
#include "../XLog.h"

void GLVideoView::SetRender(void *win) {
    XLOGI("#### GLVideoView::SetRender");
    this->win_ = win;
}

void GLVideoView::Render(XData data) {
    XLOGI("#### GLVideoView::Render");
    if (!win_)return;
    if (txt == nullptr) {
        txt = XTexture::Create();
        txt->Init(win_);
    }
    txt->Draw(data.datas, data.width, data.height);
}
