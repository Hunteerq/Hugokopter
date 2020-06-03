package com.kulmerjo.drone.hugocopter.connection.wifi.impl

import android.content.Context
import android.net.wifi.WifiManager
import com.kulmerjo.drone.hugocopter.connection.wifi.WifiService
import com.kulmerjo.drone.hugocopter.helper.ResourcesHelper

class WifiServiceImpl (private val resourcesHelper : ResourcesHelper,
                       private val context: Context)
    : WifiService {


    override fun isWifiCorrect () : Boolean {
        return getConnectedWifiSsid().equals(resourcesHelper.getDroneWifiSsid())
    }

    private fun getConnectedWifiSsid(): String? {
        val wifiManager = context.applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
        return wifiManager.connectionInfo.ssid
    }
}
