package com.kulmerjo.drone.hugocopter.control.models.info

import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable

@Serializable
data class DroneInfoData(
    @SerialName("data_type")
    val dataType : String,
    @SerialName("command")
    val command : String
)