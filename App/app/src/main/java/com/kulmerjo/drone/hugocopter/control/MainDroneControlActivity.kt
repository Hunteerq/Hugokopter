package com.kulmerjo.drone.hugocopter.control

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.MotionEvent
import android.view.View
import com.google.android.material.button.MaterialButton
import com.kulmerjo.drone.hugocopter.MainActivity
import com.kulmerjo.drone.hugocopter.R
import com.kulmerjo.drone.hugocopter.connection.drone.ConnectionService
import com.kulmerjo.drone.hugocopter.connection.drone.VideoReceiverService
import kotlinx.android.synthetic.main.activity_main_drone_control.*
import org.koin.android.ext.android.inject

class MainDroneControlActivity : AppCompatActivity() {

    private val connectionService : ConnectionService by inject()

    private val videoReceiverService: VideoReceiverService by inject()

    private val speed  = 5.0

    private val stableMoveType = "stable"

    private fun getButtonsList(): List<MaterialButton> {
        return listOf(
            button_move_forward,
            button_move_backward,
            button_move_left,
            button_move_right
        )
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main_drone_control)
        videoReceiverService.setImageView(findViewById(R.id.imageView))
        setButtonOnClickListeners()
        setButtonsOnTouchListeners()
    }

    private fun setButtonsOnTouchListeners() {
        getButtonsList().forEach { button ->
            button.setOnTouchListener {
                    view, event -> performTouchAction(view, event)
            }
        }
    }

    private fun performTouchAction(v: View, event: MotionEvent): Boolean {
        if (event.action == MotionEvent.ACTION_UP) {
            onButtonRelease()
        } else if(event.action == MotionEvent.ACTION_DOWN){
            onMoveClick(v)
        }
        return true
    }

    private fun onButtonRelease() {
        connectionService.sendControlDataToDrone(stableMoveType, 0.0)
    }

    private fun setButtonOnClickListeners() {
        button_exit.setOnClickListener { onDisconnectClick() }
    }

    override fun onDestroy() {
        super.onDestroy()
        videoReceiverService.stopThread()
    }

    fun onMoveClick(view: View) {
        val moveType = view.tag.toString()
        connectionService.sendControlDataToDrone(moveType, speed)
    }

    private fun onDisconnectClick() {
        connectionService.sendDisconnectedInfoData()
        val startActivity = Intent(this, MainActivity::class.java)
        startActivity(startActivity)
    }
}
