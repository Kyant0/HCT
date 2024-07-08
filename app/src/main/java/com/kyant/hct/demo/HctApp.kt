package com.kyant.hct.demo

import android.app.Application
import com.kyant.hct.Hct

class HctApp : Application() {
    override fun onCreate() {
        super.onCreate()
        System.loadLibrary("com.kyant.hct")
        Hct.init(useP3 = true)
    }
}
