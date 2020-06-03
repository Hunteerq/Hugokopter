package com.kulmerjo.drone.hugocopter.permission.impl

import android.Manifest
import android.app.Activity
import android.content.pm.PackageManager
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.kulmerjo.drone.hugocopter.permission.PermissionHelper

class PermissionHelperImpl : PermissionHelper{

    companion object {
        private const val INTERNET_PERMISSION_CODE : Int = 1
        private const val ACCESS_NETWORK_STATE_CODE : Int = 2
        private const val ACCESS_WIFI_STATE_CODE : Int = 3
        private const val ACCESS_FINE_LOCATION : Int = 4
    }

    override fun checkAllStartPermissions(activity: Activity) {
        getAllStartPermissionsAsMap().forEach{
                (permission, permissionCode) -> testPermission(permission, permissionCode, activity) }
    }

    private fun getAllStartPermissionsAsMap() : HashMap<String, Int> {
        return hashMapOf(
            Manifest.permission.INTERNET to INTERNET_PERMISSION_CODE,
            Manifest.permission.ACCESS_NETWORK_STATE to ACCESS_NETWORK_STATE_CODE,
            Manifest.permission.ACCESS_WIFI_STATE to ACCESS_WIFI_STATE_CODE,
            Manifest.permission.ACCESS_FINE_LOCATION to ACCESS_FINE_LOCATION
        )
    }

    private fun testPermission(permission: String, permissionCode: Int, activity: Activity) {
        if (ContextCompat.checkSelfPermission(activity, permission) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(activity, arrayOf(permission), permissionCode)
        }
    }

}
