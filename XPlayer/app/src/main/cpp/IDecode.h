//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_IDECODE_H
#define XPLAYER_IDECODE_H

#include "XParameter.h"
#include "IObserver.h"

//解码，支持硬解码
class IDecode : public IObserver {
public:
    virtual bool Open(XParameter param) = 0;//打开解码器
    virtual bool SendPacket(XData data) = 0;//发送解码
    virtual XData ReceiveFrame() = 0;//接收解码后数据
};


#endif //XPLAYER_IDECODE_H
