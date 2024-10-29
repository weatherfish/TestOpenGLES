//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_IDECODE_H
#define XPLAYER_IDECODE_H

#include "../XParameter.h"
#include "../IObserver.h"
#include <list>

//解码，支持硬解码
class IDecode : public IObserver {
public:
    virtual bool Open(XParameter param) = 0;//打开解码器
    virtual bool Open(XParameter param, bool isHard) = 0;//打开解码器
    virtual bool SendPacket(XData data) = 0;//发送解码
    virtual XData ReceiveFrame() = 0;//接收解码后数据，线程不安全，需要拷贝到其他线程

    void Update(XData data) override;

    bool isAudio = false;
    const int maxList = 100;

protected:
    void Main() override;//线程入口函数
    std::list<XData> packets; //读取缓冲区
    std::mutex packetMutex;
};


#endif //XPLAYER_IDECODE_H
