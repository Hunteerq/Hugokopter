package com.kulmerjo.drone.hugocopter.connection.verify

interface ConnectionVerifier {

    fun isConnectedToProperDevice() : Boolean

}