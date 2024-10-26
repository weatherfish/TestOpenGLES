//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_IOBSERVER_H
#define XPLAYER_IOBSERVER_H

#include <vector>
#include <mutex>
#include "XData.h"
#include "XThread.h"

class IObserver : public XThread {
public:
    virtual void Update(XData data) {}

    void AddObserver(IObserver *observer);

    void NotifyAll(XData data);

protected:
    std::vector<IObserver *> obs;
    std::mutex mutex;
};


#endif //XPLAYER_IOBSERVER_H
