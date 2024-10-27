package com.felix.xplayer

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.felix.xplayer.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        init()
        setContentView(binding.root)
//        binding.video.setOnClickListener {
//            startActivity(Intent(this, VideoActivity::class.java))
//        }
    }

    external fun init(): String

    companion object {
        init {
            System.loadLibrary("xplayer")
        }
    }
}