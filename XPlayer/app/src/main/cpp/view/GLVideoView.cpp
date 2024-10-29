//
// Created by Felix on 2024/10/27.
//

#include "GLVideoView.h"
#include "../XLog.h"

void GLVideoView::SetRender(void *win) {
    this->win_ = win;
}

void GLVideoView::Render(XData data) {
    if (!win_)return;
    if (txt == nullptr) {
        txt = XTexture::Create();
        txt->Init(win_, static_cast<XTextureType>(data.format));
    }
    txt->Draw(data.datas, data.width, data.height);
}
