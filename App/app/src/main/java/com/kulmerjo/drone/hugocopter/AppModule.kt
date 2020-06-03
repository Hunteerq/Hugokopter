package com.kulmerjo.drone.hugocopter

import com.kulmerjo.drone.hugocopter.connection.drone.ConnectionService
import com.kulmerjo.drone.hugocopter.connection.drone.VideoReceiverService
import com.kulmerjo.drone.hugocopter.connection.drone.async.tcp.AsyncTcpClient
import com.kulmerjo.drone.hugocopter.connection.drone.async.tcp.impl.AsyncTcpClientImpl
import com.kulmerjo.drone.hugocopter.connection.drone.impl.ConnectionServiceTcp
import com.kulmerjo.drone.hugocopter.connection.drone.impl.VideoReceiverServiceTcp
import com.kulmerjo.drone.hugocopter.connection.verify.ConnectionVerifier
import com.kulmerjo.drone.hugocopter.connection.verify.impl.DroneConnectionVerifier
import com.kulmerjo.drone.hugocopter.connection.wifi.WifiService
import com.kulmerjo.drone.hugocopter.connection.wifi.impl.WifiServiceImpl
import com.kulmerjo.drone.hugocopter.helper.ResourcesHelper
import com.kulmerjo.drone.hugocopter.helper.impl.ResourcesHelperImpl
import com.kulmerjo.drone.hugocopter.permission.PermissionHelper
import com.kulmerjo.drone.hugocopter.permission.impl.PermissionHelperImpl
import org.koin.android.ext.koin.androidContext
import org.koin.dsl.module

val appModule = module {
    factory<ResourcesHelper> { ResourcesHelperImpl(androidContext()) }
    single<ConnectionService> { ConnectionServiceTcp(get()) }
    single<WifiService> { WifiServiceImpl(get(), androidContext()) }
    single<PermissionHelper> { PermissionHelperImpl() }
    single<ConnectionVerifier> { DroneConnectionVerifier(get(), get())}
    single<AsyncTcpClient> {
        AsyncTcpClientImpl(
            get<ResourcesHelper>().getConfigValueAsString(R.raw.drone, ResourcesHelper.droneAddressPropertyName),
            get<ResourcesHelper>().getConfigValueAsInt(R.raw.drone, ResourcesHelper.dronePortPropertyName)
        ).also {
            it.start()
        }
    }
    single<VideoReceiverService> {
        VideoReceiverServiceTcp(
            get<ResourcesHelper>().getConfigValueAsString(R.raw.drone, ResourcesHelper.droneAddressPropertyName),
            get<ResourcesHelper>().getConfigValueAsInt(R.raw.drone, ResourcesHelper.droneVideoPortPropertyName)
        ).also{
            it.start()
        }
    }
}
