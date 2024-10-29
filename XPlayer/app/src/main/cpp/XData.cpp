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
        delete[] static_cast<unsigned char *>(data);  // Delete as array
    }
    data = nullptr;
    size = 0;
}

bool XData::Alloc(int count, const char *dat) {
    Drop();
    type = UCHAR_TYPE;
    if (count < 0)return false;
    this->size = count;
    this->data = new unsigned char[count];
    if (dat) {
        memcpy(this->data, dat, count);
    }
    return true;
}
