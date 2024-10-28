//
// Created by Felix on 2024/10/26.
//

#include "IDemux.h"
#include "../XLog.h"

void IDemux::Main() {
    while (!isExit) {
        XData d = Read();
        if (d.size > 0)NotifyAll(d);
        if (d.size <= 0)break;
    }
}
