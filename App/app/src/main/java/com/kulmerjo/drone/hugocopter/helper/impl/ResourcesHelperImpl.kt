package com.kulmerjo.drone.hugocopter.helper.impl

import android.content.Context
import com.kulmerjo.drone.hugocopter.R
import com.kulmerjo.drone.hugocopter.helper.ResourcesHelper
import java.util.*

class ResourcesHelperImpl(private val context: Context): ResourcesHelper {


    override fun getDroneWifiSsid(): String {
        return getConfigValueAsString(R.raw.drone, ResourcesHelper.droneSsid)
    }

    override fun getConfigValueAsString(configFile: Int, propertyName : String) : String {
        return getConfig(configFile, propertyName)
    }

    override fun getConfigValueAsInt(configFile: Int, propertyName : String) : Int {
        return getConfig(configFile, propertyName).toInt()
    }

    private fun getConfig(configFile: Int, propertyName : String) : String {
        val rawResource = context.resources.openRawResource(configFile)
        val property = Properties()
        property.load(rawResource)
        return property.getProperty(propertyName)
    }

}
