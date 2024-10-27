//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_XDATA_H
#define XPLAYER_XDATA_H

extern "C" {
#include <libavformat/avformat.h>
}

struct XData {
//    AVPacket *packet = nullptr;
//    AVFrame *frame = nullptr;
    int size = 0;
    bool isAudio = false;

    void *data = nullptr; //AVPacket（发送）  或者AVFrame（接收）
    unsigned char *datas[8] = {nullptr}; //解码后的数据
    int width = 0;
    int height = 0;

    void Drop();


};


#endif //XPLAYER_XDATA_H
