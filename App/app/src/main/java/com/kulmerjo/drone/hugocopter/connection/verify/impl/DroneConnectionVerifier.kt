package com.kulmerjo.drone.hugocopter.connection.verify.impl

import com.kulmerjo.drone.hugocopter.connection.drone.ConnectionService
import com.kulmerjo.drone.hugocopter.connection.verify.ConnectionVerifier
import com.kulmerjo.drone.hugocopter.connection.wifi.WifiService

class DroneConnectionVerifier(
    private val wifiService: WifiService,
    private val connectionService: ConnectionService)
    : ConnectionVerifier{

    override fun isConnectedToProperDevice() : Boolean{
        return wifiService.isWifiCorrect()  && connectionService.isConnectedToDrone()
    }

}