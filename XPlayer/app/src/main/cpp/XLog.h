//
// Created by Felix on 2024/10/26.
//

#ifndef XPLAYER_XLOG_H
#define XPLAYER_XLOG_H


class XLog {

};

#ifdef ANDROID
#include <android/log.h>
#define XLOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, "XPlayer", __VA_ARGS__)
#define XLOGI(...)  __android_log_print(ANDROID_LOG_INFO, "XPlayer", __VA_ARGS__)
#define XLOGE(...)  __android_log_print(ANDROID_LOG_ERROR, "XPlayer", __VA_ARGS__)
#else

#endif
#endif //XPLAYER_XLOG_H
