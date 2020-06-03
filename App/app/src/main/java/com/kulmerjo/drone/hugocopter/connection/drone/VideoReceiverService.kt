package com.kulmerjo.drone.hugocopter.connection.drone

import android.widget.ImageView

interface VideoReceiverService {

    fun setImageView(imageView: ImageView)

    fun stopThread()

    fun reconnect(): Boolean
}