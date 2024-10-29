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

    bool Init(void *win) override {
        if (!win) {
            XLOGE("#### CXTexture Init failed win is null");
            return false;
        }
        if (!XEGL::Get()->Init(win))return false;
        shader.Init();
        return true;
    }

    void Draw(unsigned char **data, int width, int height) override {
        shader.GetTexture(0, width, height, data[0]);
        shader.GetTexture(1, width / 2, height / 2, data[1]);
        shader.GetTexture(2, width / 2, height / 2, data[2]);

        shader.Draw();
        XEGL::Get()->Draw();
    }
};

XTexture *XTexture::Create() {
    return new CXTexture();
}
