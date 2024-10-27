//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_FFDECODE_H
#define XPLAYER_FFDECODE_H


#include "../XParameter.h"
#include "IDecode.h"

class FFDecode : public IDecode {
public:
    bool Open(XParameter param) override;//打开解码器
    bool SendPacket(XData data) override;

    XData ReceiveFrame() override;

protected:
    AVCodecContext *codec = nullptr;
    AVFrame *frame = nullptr;
};


#endif //XPLAYER_FFDECODE_H
