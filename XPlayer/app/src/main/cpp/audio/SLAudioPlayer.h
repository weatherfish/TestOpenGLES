//
// Created by Felix Wang on 2024/10/28.
//

#ifndef XPLAYER_SLAUDIOPLAYER_H
#define XPLAYER_SLAUDIOPLAYER_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "IAudioPlayer.h"

class SLAudioPlayer : public IAudioPlayer {
public:
    bool StartPlay(XParameter out) override;

    static void PlayCallback(void* bufQ);

protected:
    SLEngineItf engine = nullptr;
    SLObjectItf engineSL = nullptr;
    SLObjectItf mix = nullptr;
    SLObjectItf player = nullptr;
    SLPlayItf implPlayer = nullptr;
    SLAndroidSimpleBufferQueueItf pcmQue = nullptr;
};


#endif //XPLAYER_SLAUDIOPLAYER_H
