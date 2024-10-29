//
// Created by Felix on 2024/10/26.
//

#include "FFDecode.h"
#include "../XLog.h"

extern "C" {
#include <libavcodec/jni.h>
}

bool FFDecode::Open(XParameter param) {
    this->Open(param, false);
}

bool FFDecode::Open(XParameter param, bool isHard) {
    AVCodecParameters *p = param.param;
    //查找解码器
    const AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if (isHard) {
        cd = avcodec_find_decoder_by_name("h264_mediacodec");//
    }
    if (!cd) {
        XLOGE("#### avcodec_find_decoder %d failed isHard = %d", p->codec_id, isHard);
        return false;
    }
    //创建解码上下文，并复制参数
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec, p);

    codec->thread_count = 8;//多线程解码

    //打开解码器
    int re = avcodec_open2(codec, cd, nullptr);
    if (re != 0) {
        XLOGE("#### avcodec_open2 error %s", av_err2str(re));
        return false;
    }

    if (codec->codec_type == AVMEDIA_TYPE_AUDIO) {
        this->isAudio = true;
    } else if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        this->isAudio = false;
    } else {
        //都不是的处理
    }

    return true;
}

bool FFDecode::SendPacket(XData data) {
    if (!data.data || data.size < 0)return false;
    if (!codec) {
        return false;
    }
    int re = avcodec_send_packet(codec, reinterpret_cast<AVPacket *>(data.data));
    if (re != 0) {
        XLOGE("#### avcodec_send_packet error %s", av_err2str(re));
        return false;
    }
    return true;
}

XData FFDecode::ReceiveFrame() {
    XData d;
    if (!codec) {
        return d;
    }
    if (!frame) {
        frame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(codec, frame);
    if (re != 0) {
//        XLOGE("avcodec_receive_frame error %s", av_err2str(re));
        return d;
    }
    d.data = frame;
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        d.width = frame->width;
        d.height = frame->height;
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
    } else if (codec->codec_type == AVMEDIA_TYPE_AUDIO) {
        //样本字节数 * 单通道样本数 * 通道数
        d.size = av_get_bytes_per_sample(static_cast<AVSampleFormat>(frame->format)) *
                 frame->nb_samples * 2;
    }
    d.format = frame->format;
//    XLOGI("#### frame format is %d", frame->format);
    memcpy(d.datas, frame->data, sizeof(frame->data));

    return d;
}

void FFDecode::InitHard(void *vm) {
    av_jni_set_java_vm(vm, nullptr);
}
