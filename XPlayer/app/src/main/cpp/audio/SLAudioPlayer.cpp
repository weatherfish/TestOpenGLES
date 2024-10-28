//
// Created by Felix Wang on 2024/10/28.
//

#include "SLAudioPlayer.h"
#include "../XLog.h"


SLEngineItf SLAudioPlayer::CreateSL() {
    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL, 0, nullptr, 0, nullptr, nullptr);
    if (re != SL_RESULT_SUCCESS) return nullptr;
    re = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) return nullptr;
    re = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    if (re != SL_RESULT_SUCCESS) return nullptr;
    return en;
}

static void PcmCall(SLAndroidSimpleBufferQueueItf bf, void *context) {
    SLAudioPlayer::PlayCallback((void *) bf);
}

bool SLAudioPlayer::StartPlay(XParameter out) {
    engine = CreateSL();
    if (!engine) {
        XLOGE("CreateSL error");
        return false;
    }

    SLresult re = 0;
    re = (*engine)->CreateOutputMix(engine, &mix, 0, nullptr, nullptr);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("CreateOutputMix error");
        return false;
    }
    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("mix Realize error");
        return false;
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outputMix, nullptr};

    SLDataLocator_AndroidSimpleBufferQueue queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            static_cast<SLuint32>(out.channels),
            static_cast<SLuint32>(out.sample_rate * 1000),
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSource ds = {&queue, &pcm};

    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*engine)->CreateAudioPlayer(engine, &player, &ds, &audioSink,
                                      sizeof(ids) / sizeof(SLInterfaceID), ids, req);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("CreateAudioPlayer error");
        return false;
    }
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    re = (*player)->GetInterface(player, SL_IID_PLAY, &implPlayer);
    if (re != SL_RESULT_SUCCESS) {
        XLOGE("player GetInterface error");
        return false;
    }
    (*pcmQue)->RegisterCallback(pcmQue, PcmCall, this);
    (*implPlayer)->SetPlayState(implPlayer, SL_PLAYSTATE_PLAYING);
    (*pcmQue)->Enqueue(pcmQue, "", 1);
    XLOGI("SLAudioPlay startPlay success");
    return true;
}

void SLAudioPlayer::PlayCallback(void *bufQ) {
    if (!bufQ)return;
    auto bf = static_cast<SLAndroidSimpleBufferQueueItf>(bufQ);

}
