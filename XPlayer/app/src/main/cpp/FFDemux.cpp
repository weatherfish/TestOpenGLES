//
// Created by Felix on 2024/10/26.
//

#include "FFDemux.h"
#include "XLog.h"


bool FFDemux::Open(const char *url) {
    XLOGI("## Open file url %s", url);
    int re = avformat_open_input(&ic, url, nullptr, nullptr);
    if (re != 0) {
        XLOGE("## avformat_open_input error %s", av_err2str(re));
        return false;
    }

    re = avformat_find_stream_info(ic, nullptr);
    if (re != 0) {
        XLOGE("##a vformat_find_stream_info error %s", av_err2str(re));
        return false;
    }
    this->totalMs = ic->duration / (AV_TIME_BASE / 1000);
    XLOGI("## totalMS = %lu", this->totalMs);

    GetVParam();
    GetAParam();
    return true;
}

XData FFDemux::Read() {
    XData d;
    if (!ic)return d;
    AVPacket *packet = av_packet_alloc();
    int re = av_read_frame(ic, packet);
    if (re != 0) {
        XLOGE("av_read_frame error %s", av_err2str(re));
        av_packet_free(&packet);
        return d;
    }
//    XLOGI("packet->size = %d, packet->pts = %lu", packet->size, packet->pts);
    d.packet = packet;
    d.size = packet->size;
    if (packet->stream_index == audioStream) {
        d.isAudio = true;
    } else if (packet->stream_index == videoStream) {
        d.isAudio = false;
    } else {//既不是音频也不是视频
        av_packet_free(&packet);
        return d;
    }
    return d;
}

FFDemux::FFDemux() {
    static bool isFirst = true;
    if (isFirst) {
        isFirst = false;
    }
}

XParameter FFDemux::GetVParam() {
    XParameter p = {};
    if (!ic) {
        XLOGE("GetVParam ic is null");
        return p;
    }
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0); //获取视频流索引
    if (re < 0) {
        //找不到索引
        XLOGE("av_find_best_stream video failed");
        return p;
    }
    p.param = ic->streams[re]->codecpar;
    videoStream = re;
    return p;
}

XParameter FFDemux::GetAParam() {
    XParameter p = {};
    if (!ic) {
        XLOGE("GetAParam ic is null");
        return p;
    }
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0); //获取视频流索引
    if (re < 0) {
        //找不到索引
        XLOGE("av_find_best_stream audio failed");
        return p;
    }
    p.param = ic->streams[re]->codecpar;
    audioStream = re;
    return p;
}
