//
// Created by Felix on 2024/10/25.
//

#ifndef TESTOPENSL_LOG_H
#define TESTOPENSL_LOG_H

#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "opengl", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "opengl", __VA_ARGS__)

#endif //TESTOPENSL_LOG_H
