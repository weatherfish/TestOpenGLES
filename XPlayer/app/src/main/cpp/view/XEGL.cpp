//
// Created by Felix on 2024/10/27.
//

#include "XEGL.h"
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include "../XLOG.h"

class CXEGL : public XEGL {
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;

    static void checkEGLError(const char *operation) {
        EGLint error;
        while ((error = eglGetError()) != EGL_SUCCESS) {
            XLOGE("EGL error after %s(): 0x%x", operation, error);
        }
    }

    bool Init(void *win) override {
        auto *window = static_cast<ANativeWindow *>(win);
        //初始化EGL
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            XLOGE("eglGetDisplay error");
            return false;
        }
        eglInitialize(display, nullptr, nullptr);
        checkEGLError("eglInitialize");

        EGLConfig config;
        EGLint numConfigs;
        EGLint configSpec[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };

        eglChooseConfig(display, configSpec, &config, 1, &numConfigs);
        checkEGLError("eglChooseConfig");

        surface = eglCreateWindowSurface(display, config, window, nullptr);
        if (surface == EGL_NO_SURFACE) {
            XLOGE("eglCreateWindowSurface error");
            return false;
        }
        //打开上下文
        EGLint contextAttributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttributes);
        if (context == EGL_NO_CONTEXT) {
            XLOGE("eglCreateContext error");
            return false;
        }

        eglMakeCurrent(display, surface, surface, context);
        checkEGLError("eglMakeCurrent");

        XLOGI("#### EGL Init success");

        return true;
    }

    void Draw() override {
        if (display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) {
            XLOGI("#### display or surface is null");
            return;
        }
        eglSwapBuffers(display, surface);
    }
};

XEGL *XEGL::Get() {
    static CXEGL egl;
    return &egl;
}
