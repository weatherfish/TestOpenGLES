//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_IDEMUX_H
#define XPLAYER_IDEMUX_H


#include <cstdint>
#include "../XData.h"
#include "../IObserver.h"
#include "../XParameter.h"

class IDemux : public IObserver {
public:
    virtual bool Open(const char *url) = 0;

    virtual XParameter GetVParam() = 0; //获取视频参数
    virtual XParameter GetAParam() = 0; //获取音频参数

    virtual XData Read() = 0; //数据由调用者清理

protected:
    void Main() override;

public:
    int64_t totalMs = 0; //总时长，毫秒
};

#endif //XPLAYER_IDEMUX_H
