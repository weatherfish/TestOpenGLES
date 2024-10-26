//
// Created by Felix on 2024/10/26.
//

#include "XData.h"


void XData::Drop() {
    if (!packet)return;
    av_packet_free(&packet);
    packet = nullptr;
    size = 0;
}
