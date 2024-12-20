//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_XTHREAD_H
#define XPLAYER_XTHREAD_H

#include <string>

void XSleep(int mis);

class XThread {
public:
    virtual void Start(const std::string &name); //启动线程

    virtual void Stop(); //安全停止线程，isExit = true

    virtual void Main() {}; //线程主函数

protected:
    bool isExit = false;
    bool iSRunning = false;
    std::string th_name;

private:
    void ThreadMain();
};


#endif //XPLAYER_XTHREAD_H
