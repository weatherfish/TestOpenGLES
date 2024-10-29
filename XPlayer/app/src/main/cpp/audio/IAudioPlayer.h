//
// Created by Felix Wang on 2024/10/28.
//

#ifndef XPLAYER_IAUDIOPLAYER_H
#define XPLAYER_IAUDIOPLAYER_H


#include "../IObserver.h"
#include "../XParameter.h"
#include <list>

class IAudioPlayer : public IObserver {

public:
    void Update(XData data) override;

    virtual bool StartPlay(XParameter out) = 0;

    virtual XData GetData();//获取缓存数据，如没有则阻塞

    int maxFrames = 100;

protected:
    std::list<XData> frames;
    std::mutex framesMutex;
};


#endif //XPLAYER_IAUDIOPLAYER_H
