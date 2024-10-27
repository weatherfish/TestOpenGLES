//
// Created by Felix on 2024/10/26.
//

#include "IDecode.h"
#include "../XLog.h"

void IDecode::Main() {
    while (!isExit) {
        std::unique_lock<std::mutex> lock(packetMutex);
        if (packets.empty()) {
            lock.unlock();
            XSleep(1);
            continue;
        }
        XData data = packets.front(); //消费者
        packets.pop_front();
        if (this->SendPacket(data)) { //发送数据到解码数据
            while (!isExit) {
                //获取解码数据,一个数据包可能会有多个解码结果
                XData frame = ReceiveFrame();
                if (!frame.frame) break;
                XLOGI("ReceiveFrame size = %d", frame.size);
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
            lock.unlock();
            break;
        }
        lock.unlock();
        XSleep(1);
    }


}
