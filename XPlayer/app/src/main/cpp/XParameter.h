//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_XPARAMETER_H
#define XPLAYER_XPARAMETER_H

extern "C" {
#include <libavcodec/avcodec.h>
}

class XParameter {
public:
    AVCodecParameters *param = nullptr;
};


#endif //XPLAYER_XPARAMETER_H
