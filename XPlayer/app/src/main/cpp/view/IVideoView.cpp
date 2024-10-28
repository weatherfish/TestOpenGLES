//
// Created by Felix on 2024/10/27.
//

#include "IVideoView.h"
#include "../XLog.h"

void IVideoView::Update(XData data) {
    this->Render(data);
}
