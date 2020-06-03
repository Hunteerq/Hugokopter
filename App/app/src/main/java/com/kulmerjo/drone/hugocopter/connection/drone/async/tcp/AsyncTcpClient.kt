package com.kulmerjo.drone.hugocopter.connection.drone.async.tcp

import com.kulmerjo.drone.hugocopter.control.models.control.DroneControlData
import com.kulmerjo.drone.hugocopter.control.models.info.DroneInfoData

interface AsyncTcpClient {

     fun isConnected() : Boolean

     fun sendControlDataAsync(controlData : DroneControlData)

     fun sendInfoDataAsync(infoData: DroneInfoData)

}
