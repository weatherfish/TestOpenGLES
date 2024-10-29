//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_XDATA_H
#define XPLAYER_XDATA_H

extern "C" {
#include <libavformat/avformat.h>
}

enum XDataType {
    AVPACKET_TYPE = 0,
    UCHAR_TYPE = 1,
};

struct XData {
//    AVPacket *packet = nullptr;
//    AVFrame *frame = nullptr;

    int type = 0;
    int size = 0;
    bool isAudio = false;

    void *data = nullptr; //AVPacket（发送）  或者AVFrame（接收）
    unsigned char *datas[8] = {nullptr}; //解码后的数据
    int width = 0;
    int height = 0;

    bool Alloc(int size, const char *data = nullptr);

    void Drop();


};


#endif //XPLAYER_XDATA_H
