//
// Created by Felix Wang on 2024/10/28.
//

#include "IAudioPlayer.h"

void IAudioPlayer::Update(XData data) {
    //加入缓冲队列
    if (data.size <= 0 || !data.data)return;
    while (!isExit) {
        std::unique_lock<std::mutex> lock(framesMutex);
        if (frames.size() > maxFrames) {
            lock.unlock();
            XSleep(1);
            continue;
        }
        frames.push_back(data);
        break;
    }
}

XData IAudioPlayer::GetData() {
    XData d;
    while (!isExit) {
        std::unique_lock<std::mutex> lock(framesMutex);
        if (!frames.empty()) {
            d = frames.front();
            frames.pop_front();
            lock.unlock();
            return d;
        }
        lock.unlock();
        XSleep(1);
    }
    return d;
}
