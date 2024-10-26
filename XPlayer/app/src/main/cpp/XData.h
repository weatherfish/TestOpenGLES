//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_XDATA_H
#define XPLAYER_XDATA_H

extern "C" {
#include <libavformat/avformat.h>
}

struct XData {
    AVPacket *packet = nullptr;
    AVFrame *frame = nullptr;
    int psize = 0;
    int fsize = 0;

    void Drop();
};


#endif //XPLAYER_XDATA_H