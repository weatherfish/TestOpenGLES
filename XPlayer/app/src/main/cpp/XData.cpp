//
// Created by Felix on 2024/10/26.
//

#include "XData.h"


void XData::Drop() {
    if (!data)return;
    av_packet_free(reinterpret_cast<AVPacket **>(&data));
    data = nullptr;
    size = 0;
}
