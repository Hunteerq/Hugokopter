package com.kulmerjo.drone.hugocopter.connection.drone.impl

import com.kulmerjo.drone.hugocopter.connection.drone.ConnectionService
import com.kulmerjo.drone.hugocopter.control.models.control.DroneControlData
import com.kulmerjo.drone.hugocopter.connection.drone.async.tcp.AsyncTcpClient
import com.kulmerjo.drone.hugocopter.control.models.EDataTypes
import com.kulmerjo.drone.hugocopter.control.models.info.DroneInfoData
import com.kulmerjo.drone.hugocopter.control.models.info.EInfoTypes


class ConnectionServiceTcp(private val tcpClient : AsyncTcpClient) : ConnectionService {

    override fun isConnectedToDrone(): Boolean {
        return tcpClient.isConnected()
    }

    override fun sendControlDataToDrone(moveType: String, velocity: Double) {
        val controlData = DroneControlData(EDataTypes.CONTROL.type, moveType, velocity)
        tcpClient.sendControlDataAsync(controlData)
    }

    override fun sendConnectedInfoData() {
        sendInfoDataToDrone(EInfoTypes.CONNECT.type)
    }

    override fun sendDisconnectedInfoData() {
        sendInfoDataToDrone(EInfoTypes.DISCONNECT.type)
    }

    private fun sendInfoDataToDrone(infoType: String) {
        val infoData = DroneInfoData(EDataTypes.INFO.type, infoType)
        tcpClient.sendInfoDataAsync(infoData)
    }

}