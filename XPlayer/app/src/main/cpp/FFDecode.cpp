//
// Created by Felix on 2024/10/26.
//

#include "FFDecode.h"
#include "XLog.h"

bool FFDecode::Open(XParameter param) {
    AVCodecParameters *p = param.param;
    //查找解码器
    const AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if (!cd) {
        XLOGE("avcodec_find_decoder %d failed", p->codec_id);
        return false;
    }
    //创建解码上下文，并复制参数
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec, p);

    codec->thread_count = 8;//多线程解码

    //打开解码器
    int re = avcodec_open2(codec, cd, nullptr);
    if (re != 0) {
        XLOGE("avcodec_open2 error %s", av_err2str(re));
        return false;
    }

    return true;
}

bool FFDecode::SendPacket(XData data) {
    if (!data.packet || data.size < 0)return false;
    if (!codec) {
        return false;
    }
    int re = avcodec_send_packet(codec, data.packet);
    if (re != 0) {
        XLOGE("avcodec_send_packet error %s", av_err2str(re));
        return false;
    }
    return true;
}

XData FFDecode::ReceiveFrame() {
    XData d;
    if (!codec) {
        return d;
    }
    if (!frame)frame = av_frame_alloc();
    int re = avcodec_receive_frame(codec, frame);
    if (re != 0) {
        XLOGE("avcodec_receive_frame error %s", av_err2str(re));
        return d;
    }
    d.frame = frame;
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
    }

    return d;
}
