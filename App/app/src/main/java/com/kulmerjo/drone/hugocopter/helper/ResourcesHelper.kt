package com.kulmerjo.drone.hugocopter.helper


/**
 * @author Artur Kulig.
 * Helper which can read all resource files and return their properties
 * Bean is declared in AppModule and can be injected anywhere.
 */
interface ResourcesHelper {


    /**
     * All declared properties names
     */
    companion object {
        const val droneAddressPropertyName = "drone.connection.address"
        const val dronePortPropertyName = "drone.connection.port"
        const val droneVideoPortPropertyName = "drone.connection.video.port"
        const val droneSsid = "drone.connection.ssid"
    }

    /** Method which returns drone ssid
     */
    fun getDroneWifiSsid() : String

    /** Method which returns value as String of property from specified config file
     * @param configFile - file which will be read
     * @param propertyName - property which will be read from configFile
     */
    fun getConfigValueAsString(configFile: Int, propertyName : String) : String

    /** Method which returns value as Integer  of property from specified config file
     * @param configFile - file which will be read
     * @param propertyName - property which will be read from configFile
     */
    fun getConfigValueAsInt(configFile: Int, propertyName : String) : Int

}
