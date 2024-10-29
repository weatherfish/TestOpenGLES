#include <jni.h>
#include <string>
#include "demux/FFDemux.h"
#include "XLog.h"
#include "decode/IDecode.h"
#include "decode/FFDecode.h"
#include "view/XEGL.h"
#include "view/XShader.h"
#include "view/IVideoView.h"
#include "view/GLVideoView.h"
#include "audio/FFResample.h"
#include "audio/IAudioPlayer.h"
#include "audio/SLAudioPlayer.h"
#include <android/native_window_jni.h>
#include <EGL/egl.h>

class TestOb : public IObserver {
public:
    void Update(XData data) override {
//        XLOGI("#### Test OB Updata data size %d", data.size);
    }
};

IVideoView *view = nullptr;

extern "C" JNIEXPORT jstring JNICALL
Java_com_felix_xplayer_MainActivity_init(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *res) {
    FFDecode::InitHard(vm);

    IDemux *demux = new FFDemux();
    demux->Open("/sdcard/sample.mp4");

    IDecode *vdecode = new FFDecode();
    vdecode->Open(demux->GetVParam(), true);
    demux->AddObserver(vdecode);

    IDecode *adecode = new FFDecode();
    adecode->Open(demux->GetAParam());
    demux->AddObserver(adecode);

    view = new GLVideoView();
    vdecode->AddObserver(view);

    IResample *resample = new FFResample();
    XParameter outParam = demux->GetAParam();
    resample->Open(demux->GetAParam(), outParam);
    adecode->AddObserver(resample);

    IAudioPlayer *audioPlayer = new SLAudioPlayer();
    audioPlayer->StartPlay(outParam);
    resample->AddObserver(audioPlayer);

    demux->Start("demux");
    vdecode->Start("vdecode");
    adecode->Start("adecode");
//    XData d = demux->Read();
//    XLOGI("#### Read data size is %d", d.size);
    return JNI_VERSION_1_6;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_felix_xplayer_view_XPlay_initView(JNIEnv *env, jobject thiz, jobject surface) {
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    view->SetRender(win);
//    auto *egl = XEGL::Get();
//    egl->Init(win);
//    XShader shader;
//    shader.Init();
}