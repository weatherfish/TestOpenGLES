//
// Created by Felix on 2024/10/26.
//

#ifndef TESTOPENGLANDROID_SHADER_UTILS_H
#define TESTOPENGLANDROID_SHADER_UTILS_H

#include <GLES2/gl2.h>

#define GET_STR(x) #x

// 顶点着色器
static const char *vertexShaderSrc = GET_STR(
        attribute vec4 aPosition;
        attribute vec2 aTextCord;

        varying vec2 vTexCord;
        void main() {
            vTexCord = vec2(aTextCord.x, 1.0 - aTextCord.y);
            gl_Position = aPosition;
        }
);

// 片段着色器
static const char *fragmentShaderSrc = GET_STR(
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

            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.5806, 0.0) * yuv;

//            gl_FragColor = vec4(rgb, 1.0);
            gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
);

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
GLuint InitShader(const char *code, GLint type) {
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        LOGE("Failed to create shader");
        return 0;
    }

    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus != GL_TRUE) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        LOGE("Error compiling shader: %s", infoLog);
        glDeleteShader(shader); // Delete the shader as it's not usable
        return 0;
    }

    return shader;
}

#endif //TESTOPENGLANDROID_SHADER_UTILS_H
