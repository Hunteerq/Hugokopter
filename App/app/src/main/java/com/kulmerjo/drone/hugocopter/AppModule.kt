package com.kulmerjo.drone.hugocopter

import com.kulmerjo.drone.hugocopter.connection.drone.ConnectionService
import com.kulmerjo.drone.hugocopter.connection.drone.async.tcp.AsyncTcpClient
import com.kulmerjo.drone.hugocopter.connection.drone.async.tcp.impl.AsyncTcpClientImpl
import com.kulmerjo.drone.hugocopter.connection.drone.impl.ConnectionServiceTcp
import com.kulmerjo.drone.hugocopter.connection.wifi.WifiService
import com.kulmerjo.drone.hugocopter.connection.wifi.impl.WifiServiceImpl
import com.kulmerjo.drone.hugocopter.helper.ResourcesHelper
import com.kulmerjo.drone.hugocopter.helper.impl.ResourcesHelperImpl
import com.kulmerjo.drone.hugocopter.permission.PermissionHelper
import com.kulmerjo.drone.hugocopter.permission.impl.PermissionHelperImpl
import org.koin.dsl.module

val appModule = module {
    factory<ResourcesHelper> { ResourcesHelperImpl() }
    single<ConnectionService> { ConnectionServiceTcp(get()) }
    single<WifiService> { WifiServiceImpl(get()) }
    single<PermissionHelper> { PermissionHelperImpl() }
    single<AsyncTcpClient> {
        AsyncTcpClientImpl(
        ).also {
            it.start()
        }
    }
}
