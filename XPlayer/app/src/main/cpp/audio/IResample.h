//
// Created by Felix on 2024/10/27.
//

#ifndef XPLAYER_IRESAMPLE_H
#define XPLAYER_IRESAMPLE_H


#include "../IObserver.h"
#include "../XParameter.h"

class IResample : public IObserver {
public:
    virtual bool Open(XParameter in, XParameter out = XParameter()) = 0;
};


#endif //XPLAYER_IRESAMPLE_H