//
// Created by Felix on 2024/10/27.
//

#ifndef XPLAYER_FFRESAMPLE_H
#define XPLAYER_FFRESAMPLE_H

extern "C" {
#include <libswresample/swresample.h>
#include <libavutil/avutil.h>
}

#include "IResample.h"

class FFResample : public IResample {
public:
    bool Open(XParameter in) override;

    bool Open(XParameter in, XParameter out) override;

    XData Resample(XData indata) override;

    void Update(XData data) override;

protected:
    SwrContext *swrContext;
};


#endif //XPLAYER_FFRESAMPLE_H
