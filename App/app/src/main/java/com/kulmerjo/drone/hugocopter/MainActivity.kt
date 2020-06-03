package com.kulmerjo.drone.hugocopter

import android.content.Intent
import android.os.Bundle
import android.view.animation.Animation
import android.view.animation.AnimationUtils
import androidx.appcompat.app.AppCompatActivity
import com.kulmerjo.drone.hugocopter.connection.drone.ConnectionService
import com.kulmerjo.drone.hugocopter.connection.verify.ConnectionVerifier
import com.kulmerjo.drone.hugocopter.control.MainDroneControlActivity
import com.kulmerjo.drone.hugocopter.notification.NotConnectedToDroneActivity
import com.kulmerjo.drone.hugocopter.permission.PermissionHelper
import kotlinx.android.synthetic.main.activity_main.*
import org.koin.android.ext.android.inject


/**
 * @author hunteerq
 * Method provides logo and button which starts the control mode
 **/
class MainActivity : AppCompatActivity() {

    private val connectionVerifier : ConnectionVerifier by inject()

    private val connectionService : ConnectionService by inject()

    private val permissionHelper : PermissionHelper by inject()

    private val animationDuration = 700L


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        permissionHelper.checkAllStartPermissions(this)
        setButtonsOnClickMethods()
        startAnimations()
        connectionVerifier.isConnectedToProperDevice()
    }

    private fun setButtonsOnClickMethods() {
        button_start.setOnClickListener{ onClickMethod() }
    }

    private fun startAnimations() {
        val slideInLeftLogoAnimation = AnimationUtils.loadAnimation(this, android.R.anim.slide_in_left)
        setAnimation(slideInLeftLogoAnimation)
    }

    private fun setAnimation(animation: Animation) {
        animation.duration = animationDuration
        logo_full.startAnimation(animation)
        button_start.startAnimation(animation)
    }

    private fun onClickMethod() {
        val nextIntent = isConnectedToDroneIntent()
        finish()
        startActivity(nextIntent)
    }

    private fun isConnectedToDroneIntent(): Intent {
        return if (connectionVerifier.isConnectedToProperDevice()) {
            connectionService.sendConnectedInfoData()
            Intent(this, MainDroneControlActivity::class.java) }
        else {
            Intent(this, NotConnectedToDroneActivity::class.java)
        }
    }
}
