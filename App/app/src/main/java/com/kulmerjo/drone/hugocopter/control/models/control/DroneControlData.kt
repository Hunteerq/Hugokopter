package com.kulmerjo.drone.hugocopter.control.models.control

import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable

@Serializable
data class DroneControlData (
    @SerialName("data_type")
    val dataType : String,
    @SerialName("control_type")
    val controlType: String,
    @SerialName("speed")
    val speed : Double
)