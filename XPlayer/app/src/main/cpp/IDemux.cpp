//
// Created by Felix on 2024/10/26.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::Main() {
    while (!isExit) {
        XData d = Read();
//        XLOGI("IDemux read %d", d.size);
        if (d.psize > 0)NotifyAll(d);
        if (d.psize <= 0)break;
    }
}