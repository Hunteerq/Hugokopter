package com.kulmerjo.drone.hugocopter.connection.drone


interface ConnectionService {

    fun isConnectedToDrone(): Boolean

    fun sendControlDataToDrone(moveType: String, velocity: Double)

    fun sendConnectedInfoData()

    fun sendDisconnectedInfoData()


}