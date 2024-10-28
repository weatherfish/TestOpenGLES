//
// Created by Felix on 2024/10/26.
//

#include "IDecode.h"
#include "../XLog.h"

void IDecode::Main() {
    while (!isExit) {
        XLOGI("#### IDecode::Main ");
        std::unique_lock<std::mutex> lock(packetMutex);
        if (packets.empty()) {
            XLOGI("#### IDecode::Main packets empty");
            lock.unlock();
            XSleep(1);
            continue;
        }
        XData data = packets.front(); //消费者
        XLOGI("#### IDecode::Main packets front");
        packets.pop_front();
        if (this->SendPacket(data)) { //发送数据到解码数据
            while (!isExit) {
                //获取解码数据,一个数据包可能会有多个解码结果
                XData frame = ReceiveFrame();
                if (!frame.data) break;
                XLOGI("#### IDecode::Main ReceiveFrame size = %d", frame.size);
                this->NotifyAll(frame);//发送数据给观察者
            }
        }
        data.Drop();
    }
}

void IDecode::Update(XData data) {
    //判断
    if (data.isAudio != isAudio) {
        return;
    }
    while (!isExit) {
        std::unique_lock<std::mutex> lock(packetMutex);
        if (packets.size() < maxList) {
            packets.push_back(data); //生产者
            XLOGI("#### IDecode::Update packets push_back ");
            lock.unlock();
            break;
        }
        lock.unlock();
        XSleep(1);
    }
}
