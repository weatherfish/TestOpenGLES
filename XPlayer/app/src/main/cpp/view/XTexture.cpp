//
// Created by Felix on 2024/10/27.
//

#include "XTexture.h"
#include "../XLog.h"
#include "XEGL.h"
#include "XShader.h"

class CXTexture : public XTexture {
public:
    XShader shader;
    XTextureType type_;

    bool Init(void *win, XTextureType type) override {
        this->type_ = type;
        if (!win) {
            XLOGE("#### CXTexture Init failed win is null");
            return false;
        }
        if (!XEGL::Get()->Init(win))return false;
        shader.Init(type);
        return true;
    }

    void Draw(unsigned char **data, int width, int height) override {
        shader.GetTexture(0, width, height, data[0], false);
        if (type_ == XTextureType::TEXTURE_YUV420P) {
            shader.GetTexture(1, width / 2, height / 2, data[1], false);
            shader.GetTexture(2, width / 2, height / 2, data[2], false);
        } else {
            shader.GetTexture(1, width / 2, height / 2, data[1], true);
        }

        shader.Draw();
        XEGL::Get()->Draw();
    }
};

XTexture *XTexture::Create() {
    return new CXTexture();
}
