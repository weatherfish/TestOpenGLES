//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_FFDEMUX_H
#define XPLAYER_FFDEMUX_H

#include "IDemux.h"

extern "C" {
#include "libavformat/avformat.h"
}

class FFDemux : public IDemux {
public:
    FFDemux();

    bool Open(const char *url) override;

    XParameter GetVParam() override;

    XParameter GetAParam() override;

    XData Read() override;

private:
    AVFormatContext *ic = nullptr;
    int audioStream = 1;
    int videoStream = 0;
};


#endif //XPLAYER_FFDEMUX_H
