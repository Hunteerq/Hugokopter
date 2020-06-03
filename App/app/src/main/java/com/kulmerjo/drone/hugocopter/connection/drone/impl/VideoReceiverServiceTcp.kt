package com.kulmerjo.drone.hugocopter.connection.drone.impl

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.widget.ImageView
import com.kulmerjo.drone.hugocopter.connection.drone.VideoReceiverService
import java.io.BufferedInputStream
import java.io.DataInputStream
import java.net.Socket
import java.util.concurrent.locks.Lock
import java.util.concurrent.locks.ReentrantLock
import kotlin.concurrent.withLock

class VideoReceiverServiceTcp(private val ipAddress: String, private val port: Int) :Thread(), VideoReceiverService {

    private var imageView: ImageView? = null
    private var reader: DataInputStream? = null
    private var isStopped: Boolean = false
    private var socket: Socket? = null
    private val packageHeaderSize: Int = 4
    private val imageReadRetryDelay: Long = 100
    private val imageReadMaxRetryAttempts: Int = 50
    private var imageLock: Lock = ReentrantLock()

    override fun run() {
        super.run()
        reconnect()
        receiveVideo()
        disconnect()
    }

    override fun reconnect(): Boolean {
        disconnect()
        return runCatching {
            socket = Socket(ipAddress, port)
            socket?.let{ reader = DataInputStream(BufferedInputStream(it.getInputStream())) }
        }.isSuccess
    }

    private fun disconnect() {
        socket?.close()
    }

    private fun receiveVideo() {
        while (!isStopped) {
            showFrame()
            if (isConnected() == false || isClosed() == true || socket == null) {
                reconnect()
            }
        }
    }

    private fun showFrame() {
        kotlin.runCatching {
            val imageSize = readImageSize()
            val image = readImage(imageSize)
            val bitMap: Bitmap? = convertByteArrayToBitmap(image)
            updateImageView(bitMap)
        }
    }

    private fun readImageSize(): Int? {
        return if (reader != null || reader!!.available() >= packageHeaderSize) reader?.readInt() else null
    }

    private fun readImage(size: Int?): ByteArray? {
        if (size == null || !isImageAvailable(size)) {
            return null
        }
        val byteArray = ByteArray(size)
        reader?.readFully(byteArray, 0, size)
        return runCatching {
            if (byteArray.isNotEmpty()) byteArray else null
        }.getOrNull()
    }

    private fun isImageAvailable(size: Int): Boolean {
        return reader?.run {
            waitForImage(this, size)
        } ?: false
    }

    private fun waitForImage(reader: DataInputStream, size : Int): Boolean {
        var numberOfAttempts = 0
        while (reader.available() < size) {
            numberOfAttempts++
            if (numberOfAttempts > imageReadMaxRetryAttempts) return false
            sleep(imageReadRetryDelay)
        }
        return true
    }

    private fun convertByteArrayToBitmap(image: ByteArray?): Bitmap? {
        return image?.size?.let { BitmapFactory .decodeByteArray(image, 0, image.size) }
    }

    private fun updateImageView(bitMap: Bitmap?) {
        bitMap?.let {
                bitmap -> imageView?.let {
                iv -> imageLock.withLock{ createImageViewPost(iv, bitmap) }
            }
        }
    }

    private fun createImageViewPost(imageView: ImageView, bitMap: Bitmap) {
        imageView.post {
            imageView.setImageBitmap(Bitmap.createScaledBitmap(bitMap, imageView.width, imageView.height, false))
        }
    }

    private fun isConnected(): Boolean? {
        return socket?.isConnected
    }

    private fun isClosed(): Boolean? {
        return socket?.isClosed
    }

    override fun stopThread() {
        isStopped = true
    }

    override fun setImageView(imageView: ImageView) {
        imageLock.withLock { this.imageView = imageView }
    }
}