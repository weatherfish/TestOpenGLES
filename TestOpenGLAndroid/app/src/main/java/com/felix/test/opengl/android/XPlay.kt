package com.felix.test.opengl.android

import android.content.Context
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView

//
//  Created by wangqiang on 2024/10/26.
//  TestOpenGLAndroid
//
//  XPlay
class XPlay : SurfaceView, Runnable, SurfaceHolder.Callback {
    constructor(context: Context?) : super(context)

    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)

    override fun run() {
        open("/sdcard/demo.yuv", holder.surface)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        val renderThread = Thread(this)
        renderThread.start()
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, w: Int, h: Int) {

    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {

    }

    private external fun open(url: String, surface: Any)
}
