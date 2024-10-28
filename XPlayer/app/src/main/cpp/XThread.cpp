//
// Created by Felix on 2024/10/26.
//

#include "XThread.h"
#include "XLog.h"
#include <thread>
#include <chrono>

void XSleep(int mis) {
    std::chrono::milliseconds du(mis);
    std::this_thread::sleep_for(du);
}

void XThread::Start(const std::string &name) {
    this->th_name = name;
    isExit = false;
    XLOGI("#### Thread %s Start", this->th_name.c_str());
    std::thread thread(&XThread::ThreadMain, this);
    thread.detach();//放弃对新创建线程的控制
}

void XThread::Stop() {
    isExit = true;
    for (int i = 0; i < 200; ++i) {
        if (!iSRunning) {
            XLOGI("#### Thread %s stop success", this->th_name.c_str());
            return;
        }
        XSleep(1);
    }
    XLOGI("#### Thread stop time out");
}

void XThread::ThreadMain() {
    iSRunning = true;
    XLOGI("#### XThread %s main in", this->th_name.c_str());
    Main();
    XLOGI("#### XThread %s main out", this->th_name.c_str());
    iSRunning = false;
}
