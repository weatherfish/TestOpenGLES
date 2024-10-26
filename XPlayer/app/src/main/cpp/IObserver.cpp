//
// Created by Felix on 2024/10/26.
//

#include "IObserver.h"

void IObserver::AddObserver(IObserver *observer) {
    if (!observer)return;
    std::unique_lock<std::mutex> lock(mutex);
    obs.push_back(observer);
}

void IObserver::NotifyAll(XData data) {
    std::unique_lock<std::mutex> lock(mutex);
    for (auto &ob: obs) {
        ob->Update(data);
    }
}
