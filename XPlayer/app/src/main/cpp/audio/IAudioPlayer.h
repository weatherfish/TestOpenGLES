//
// Created by Felix Wang on 2024/10/28.
//

#ifndef XPLAYER_IAUDIOPLAYER_H
#define XPLAYER_IAUDIOPLAYER_H


#include "../IObserver.h"
#include "../XParameter.h"

class IAudioPlayer : public IObserver {

public:
    void Update(XData data) override;

    virtual bool StartPlay(XParameter out) = 0;

};


#endif //XPLAYER_IAUDIOPLAYER_H
