package com.kulmerjo.drone.hugocopter.notification

import android.content.Intent
import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.kulmerjo.drone.hugocopter.R
import com.kulmerjo.drone.hugocopter.connection.drone.ConnectionService
import com.kulmerjo.drone.hugocopter.connection.verify.ConnectionVerifier
import com.kulmerjo.drone.hugocopter.control.MainDroneControlActivity
import kotlinx.android.synthetic.main.activity_not_connected_to_drone.*
import org.koin.android.ext.android.inject

class NotConnectedToDroneActivity : AppCompatActivity() {

    private val connectionVerifier : ConnectionVerifier by inject()

    private val connectionService : ConnectionService by inject()

    private val droneControlIntent = Intent(this, MainDroneControlActivity::class.java)


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_not_connected_to_drone)
        setButtonOnClickListeners()
    }

    private fun setButtonOnClickListeners() {
        test_connection_again_button.setOnClickListener { onClickMethod() }
    }

    private fun onClickMethod() {
        if (connectionVerifier.isConnectedToProperDevice()) {
            connectionService.sendConnectedInfoData()
            startActivity(droneControlIntent)
        } else {
            still_no_connection_text.visibility = View.VISIBLE
        }
    }

}
