#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <malloc.h>

#include "utils/log.h"
#include "utils/shader_utils.h"

void checkEGLError(const char *operation) {
    EGLint error;
    while ((error = eglGetError()) != EGL_SUCCESS) {
        LOGE("EGL error after %s(): 0x%x", operation, error);
    }
}

void checkGLError(const char *operation) {
    GLint error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOGE("GL error after %s(): 0x%x", operation, error);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_felix_test_opengl_android_XPlay_open(JNIEnv *env, jobject thiz, jstring url,
                                              jobject surface) {
    const char *url_ = env->GetStringUTFChars(url, nullptr);
    LOGD("Open url is %s", url_);

    // Open file (example comment: make sure file exists)
    FILE *fp = fopen(url_, "rb");
    if (!fp) {
        LOGE("fopen %s failed", url_);
        env->ReleaseStringUTFChars(url, url_);
        return;
    }

    // Obtain a native window from the Java Surface
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);

    // Initialize EGL display connection
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, nullptr, nullptr);
    checkEGLError("eglInitialize");

    // Define configuration attributes
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

    // Create an EGL window surface
    EGLSurface eglSurface = eglCreateWindowSurface(display, config, window, nullptr);
    checkEGLError("eglCreateWindowSurface");

    // Set up the EGL context
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    checkEGLError("eglCreateContext");

    // Bind the context to the current thread
    eglMakeCurrent(display, eglSurface, eglSurface, context);
    checkEGLError("eglMakeCurrent");

    // Shader initialization
    GLuint vertexShader = InitShader(vertexShaderSrc, GL_VERTEX_SHADER);
    GLuint fragmentShader = InitShader(fragmentShaderSrc, GL_FRAGMENT_SHADER);

    // Check shader compilation success
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        LOGE("Vertex shader compilation failed");
        return;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        LOGE("Fragment shader compilation failed");
        return;
    }

    // Program creation and linking
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        LOGE("Program linkage failed");
        return;
    }

    glUseProgram(program);

    // Buffers for YUV data
    int width = 424;
    int height = 240;
    unsigned char *buf[3] = {nullptr};

    buf[0] = new unsigned char[width * height];
    buf[1] = new unsigned char[width * height / 4];
    buf[2] = new unsigned char[width * height / 4];

    GLuint texYUV[3];
    glGenTextures(3, texYUV);

    // Main rendering loop
    for (int i = 0; i < 1000; ++i) {
        memset(buf[0], i, width * height);
        memset(buf[1], i, width * height / 4);
        memset(buf[2], i, width * height / 4);

        // Update texture with new data
        for (int j = 0; j < 3; ++j) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, texYUV[j]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            j == 0 ? width : width / 2,
                            j == 0 ? height : height / 2,
                            GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[j]);
        }

        // Perform drawing operations
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0, 0.0, 0.0, 1.0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // Use correct primitive count

        eglSwapBuffers(display, eglSurface);
    }

    // Clean up
    delete[] buf[0];
    delete[] buf[1];
    delete[] buf[2];

    eglDestroySurface(display, eglSurface);
    eglDestroyContext(display, context);
    eglTerminate(display);

    env->ReleaseStringUTFChars(url, url_);
}
