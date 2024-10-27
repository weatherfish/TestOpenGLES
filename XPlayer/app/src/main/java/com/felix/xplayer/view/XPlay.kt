package com.felix.xplayer.view

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.Surface
import android.view.SurfaceHolder

//
//  Created by wangqiang on 2024/10/27.
//  XPlayer
//
//  XPlay

class XPlay(context: Context, atts: AttributeSet) : GLSurfaceView(context, atts),
    SurfaceHolder.Callback {

    override fun surfaceCreated(holder: SurfaceHolder) {
        initView(holder.surface) //初始化egl
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {

    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, w: Int, h: Int) {

    }

    private external fun initView(surface: Surface)

}