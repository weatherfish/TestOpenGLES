#include "FFResample.h"
#include "../XData.h"
#include "../XLog.h"

bool FFResample::Open(XParameter in, XParameter out) {
    if (in.param->format) {
        swrContext = swr_alloc();
        if (!swrContext) {
            XLOGE("#### Failed to allocate SwrContext");
            return false;
        }

        AVChannelLayout out_channel_layout = AV_CHANNEL_LAYOUT_STEREO;
        int re = swr_alloc_set_opts2(&swrContext, &out_channel_layout, AV_SAMPLE_FMT_S16,
                                     out.param->sample_rate,
                                     &out_channel_layout,
                                     static_cast<AVSampleFormat>(in.param->format),
                                     in.param->sample_rate, 0, nullptr);
        if (re < 0) {
            XLOGE("#### swr_alloc_set_opts2 failed: %d", re);
            swr_free(&swrContext);
            return false;
        }

        re = swr_init(swrContext);
        if (re != 0) {
            XLOGE("#### swr init failed with error code: %d", re);
            swr_free(&swrContext);
            return false;
        }
        outChannel = in.param->ch_layout.nb_channels;
        outFormat = AV_SAMPLE_FMT_S16;
    }
    return true;
}

XData FFResample::Resample(XData indata) {
    XData out;
    if (!swrContext || indata.size < 0 || !indata.data) return out;

    auto *frame = static_cast<AVFrame *>(indata.data);
    int outSize = outChannel * frame->nb_samples *
                  av_get_bits_per_sample(
                          av_get_pcm_codec(static_cast<AVSampleFormat>(outFormat), 0));
    if (outSize < 0) return out;
    if (!out.Alloc(outSize)) {
        XLOGE("#### Failed to allocate memory for resampling");
        return out;
    }

    uint8_t *outArr[2] = {static_cast<uint8_t *>(out.data)};

    int len = swr_convert(swrContext, outArr, frame->nb_samples, frame->data, frame->nb_samples);
    if (len < 0) {
        out.Drop();
        XLOGE("#### swr_convert failed with length: %d", len);
        return out;
    }

    XLOGI("#### Resample out with size: %d", out.size);
    return out;
}

void FFResample::Update(XData data) {
    XData d = this->Resample(data);
    if (d.size > 0) {
        this->NotifyAll(d);
    }
}
