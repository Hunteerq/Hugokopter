#include <Wire.h>
#include <Drone.h>

Drone drone;


void setup() {
  Serial.begin(9600);
  wireInitialize();
  drone.initialize();
}

void wireInitialize(){
  Wire.begin();
  Wire1.begin(0x13);
  Wire1.onReceive(onCommand);
}

void loop() {
  drone.droneLoop();  
}

void onCommand(int howMany) {
    int drone_command = (int)Wire1.read();  
    int drone_speed = (int)Wire1.read();
    drone.setDirection(drone_command, drone_speed);  
}
