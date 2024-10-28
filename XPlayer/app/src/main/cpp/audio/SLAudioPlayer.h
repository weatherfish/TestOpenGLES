//
// Created by Felix Wang on 2024/10/28.
//

#ifndef XPLAYER_SLAUDIOPLAYER_H
#define XPLAYER_SLAUDIOPLAYER_H


#include "IAudioPlayer.h"

class SLAudioPlayer : public IAudioPlayer {
public:
    bool StartPlay(XParameter out) override;
};


#endif //XPLAYER_SLAUDIOPLAYER_H
