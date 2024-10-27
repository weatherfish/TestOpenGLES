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
    int width = 200;
    int height = 200;
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

    //加入顶点数据
    GLuint aPos = glGetAttribLocation(program, "aPosition");
    glEnableVertexAttribArray(aPos);
    //传递顶点
    glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, ver);

    //加入材质数据
    GLuint aTex = glGetAttribLocation(program, "aTextCord");
    glEnableVertexAttribArray(aTex);
    glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, tex);

    //材质纹理初始化
    //设置纹理层
    glUniform1i(glGetUniformLocation(program, "yTexture"), 0);
    glUniform1i(glGetUniformLocation(program, "uTexture"), 1);
    glUniform1i(glGetUniformLocation(program, "vTexture"), 2);

    //创建OpenGL纹理
    GLuint texts[3] = {0};
    glGenTextures(3, texts);

    glBindTexture(GL_TEXTURE_2D, texts[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理大小格式
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, texts[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理大小格式
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width / 2, height / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, texts[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理大小格式
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width / 2, height / 2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, nullptr);

    unsigned char *buf[3] = {nullptr};
    //纹理的修改和显示
    buf[0] = new unsigned char[width * height];
    buf[1] = new unsigned char[width * height / 4];
    buf[2] = new unsigned char[width * height / 4];

    for (int i = 0; i < 1000; ++i) {
//            memset(buf[0], i, width * height);
//            memset(buf[1], i, width * height / 4);
//            memset(buf[2], i, width * height / 4);

        if (feof(fp) == 0) {
            fread(buf[0], 1, width * height, fp);
            fread(buf[0], 1, width * height / 4, fp);
            fread(buf[0], 1, width * height / 4, fp);
        }

        glActiveTexture(GL_TEXTURE0); //激活第一个
        glBindTexture(GL_TEXTURE_2D, texts[0]); //绑定到创建的y
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                        buf[0]);

        glActiveTexture(GL_TEXTURE0 + 1); //激活第二
        glBindTexture(GL_TEXTURE_2D, texts[1]); //绑定到创建的y
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                        buf[1]);

        glActiveTexture(GL_TEXTURE0 + 2); //激活第三
        glBindTexture(GL_TEXTURE_2D, texts[2]); //绑定到创建的y
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                        buf[2]);

        glDrawArrays(GL_TRIANGLES, 0, 4);

        //窗口显示
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
