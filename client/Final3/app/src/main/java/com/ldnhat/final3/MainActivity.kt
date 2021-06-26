package com.ldnhat.final3

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.os.Looper

class MainActivity : AppCompatActivity() {

    private lateinit var handler: Handler


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        handleDelay()
    }

    private fun handleDelay(){
        handler = Handler(Looper.getMainLooper())

        handler.postDelayed({
            var intent = Intent(this, HomeActivity::class.java)
            startActivity(intent)
        }, 1000)
    }
}