//
// Created by Felix on 2024/10/27.
//

#ifndef XPLAYER_IVIDEOVIEW_H
#define XPLAYER_IVIDEOVIEW_H


#include "../XData.h"
#include "../IObserver.h"

class IVideoView : public IObserver {
public:
    virtual void SetRender(void *win) = 0;

    virtual void Render(XData data) = 0;

    void Update(XData data) override;
};


#endif //XPLAYER_IVIDEOVIEW_H
