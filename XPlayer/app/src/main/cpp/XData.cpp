//
// Created by Felix on 2024/10/26.
//

#include "XData.h"
#include <memory>

void XData::Drop() {
    if (!data)return;
    if (type == AVPACKET_TYPE) {
        av_packet_free(reinterpret_cast<AVPacket **>(&data));
    } else {
        delete data;
    }
    data = nullptr;
    size = 0;
}

bool XData::Alloc(int size, const char *data) {
    Drop();
    type = UCHAR_TYPE;
    if (size < 0)return false;
    this->data = new unsigned char[size];
    if (data) {
        memcpy(this->data, data, size);
    }
}
