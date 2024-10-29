//
// Created by Felix on 2024/10/27.
//

#include "XShader.h"
#include "../XLog.h"
#include <GLES2/gl2.h>
#include <string>

#define GET_STR(x) #x

// 顶点着色器
static const char *vertexShaderSrc = R"(
    attribute vec4 aPosition;
    attribute vec2 aTextCord;

    varying vec2 vTexCord;

    void main() {
        vTexCord = vec2(aTextCord.x, 1.0 - aTextCord.y);
        gl_Position = aPosition;
    }
)";

// 片段着色器
static const char *fragmentShaderSrc = R"(
        precision mediump float;

        varying vec2 vTexCord;

        uniform sampler2D yTexture;
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;

        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture, vTexCord).r;
            yuv.g = texture2D(uTexture, vTexCord).r - 0.5;
            yuv.b = texture2D(vTexture, vTexCord).r - 0.5;

            rgb = mat3(
                1.0, 1.0, 1.0,
                0.0, -0.39465, 2.03211,
                1.13983, -0.5806, 0.0) * yuv;

            gl_FragColor = vec4(rgb, 1.0);
        }
)";

static float ver[] = {
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
};

static float tex[] = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
};

// 初始化着色器函数
static GLuint InitShader(const char *code, GLint type) {
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        XLOGE("#### Failed to create shader");
        return 0;
    }

    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus != GL_TRUE) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        XLOGE("#### Error compiling shader: %s", infoLog);
        glDeleteShader(shader); // Delete the shader as it's not usable
        return 0;
    }

    return shader;
}

static void checkGLError(const std::string &funName) {
    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
        std::string errorString;
        switch (error) {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                errorString = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            default:
                errorString = "Unknown error";
                break;
        }
        XLOGE("#### OpenGL [%s] Error : [%s] ", funName.c_str(), errorString.c_str());
        // 再次调用以移除所有报错
        error = glGetError();
    }
}

bool XShader::Init() {
    vertexShader = InitShader(vertexShaderSrc, GL_VERTEX_SHADER);
    fragmentShader = InitShader(fragmentShaderSrc, GL_FRAGMENT_SHADER);

    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        XLOGE("#### Vertex shader compilation failed");
        return false;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        XLOGE("#### Fragment shader compilation failed");
        return false;
    }

    // Program creation and linking
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        XLOGE("#### Program linkage failed");
        return false;
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

    XLOGI("#### Init Shader success");
    return true;
}

void XShader::GetTexture(unsigned int index, int width, int height, unsigned char *buf) {
    if (texts[index] == 0) {
        //纹理初始化
        glGenTextures(1, &texts[index]);
        glBindTexture(GL_TEXTURE_2D, texts[index]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //设置纹理大小格式
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE,
                     GL_UNSIGNED_BYTE, nullptr);
    }
    glActiveTexture(GL_TEXTURE0 + index); //激活第一个
    glBindTexture(GL_TEXTURE_2D, texts[index]); //绑定到创建的y
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                    buf);
}

void XShader::Draw() {
    if (!program) {
        XLOGI("#### XShader Error program is null");
        return;
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
