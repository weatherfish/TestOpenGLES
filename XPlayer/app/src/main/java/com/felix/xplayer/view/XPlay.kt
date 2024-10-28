package com.felix.xplayer.view

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.Surface
import android.view.SurfaceHolder
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

//
//  Created by wangqiang on 2024/10/27.
//  XPlayer
//
//  XPlay

class XPlay(context: Context, atts: AttributeSet) : GLSurfaceView(context, atts),
    GLSurfaceView.Renderer, SurfaceHolder.Callback {

    override fun surfaceCreated(holder: SurfaceHolder) {
        initView(holder.surface) //初始化egl
        setRenderer(this)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {

    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, w: Int, h: Int) {

    }

    private external fun initView(surface: Surface)
    override fun onSurfaceCreated(p0: GL10?, p1: EGLConfig?) {
    }

    override fun onSurfaceChanged(p0: GL10?, p1: Int, p2: Int) {
    }

    override fun onDrawFrame(p0: GL10?) {
    }

}