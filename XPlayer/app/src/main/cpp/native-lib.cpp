#include <jni.h>
#include <string>
#include "FFDemux.h"
#include "XLog.h"
#include "IDecode.h"
#include "FFDecode.h"

class TestOb : public IObserver {
public:
    void Update(XData data) override {
//        XLOGI("Test OB Updata data size %d", data.size);
    }
};

extern "C" JNIEXPORT jstring JNICALL
Java_com_felix_xplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    auto *tob = new TestOb();
    IDemux *demux = new FFDemux();
    demux->AddObserver(tob);
    demux->Open("/sdcard/sample.mp4");

    IDecode *decode = new FFDecode();
    decode->Open(demux->GetParam());

    demux->Start();
    XSleep(3000);
    demux->Stop();
//    XData d = demux->Read();
//    XLOGI("Read data size is %d", d.size);
    return env->NewStringUTF(hello.c_str());
}