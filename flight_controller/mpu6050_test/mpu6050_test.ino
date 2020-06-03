#include <AccelGyro.h>
#include <Wire.h>

AccelGyro accelGyro;
Rotation rotation;

void setup() {
  Serial.begin(9600);
  accelGyro.Initialize();
  Serial.print("Is connected: ");
  Serial.println(accelGyro.IsConnected());
  if(!accelGyro.IsConnected()){
    Serial.println("Cannot get data from MPU6050");
    //while(1);
  }
  accelGyro.CalibrateGyro();
  Serial.println("Rotation:");
  Serial.println("x\ty\tz");
}

void loop() {
  if(Serial.available()){
    if(Serial.read() == 97){
      accelGyro.Calibrate();
    }
  }
  rotation = accelGyro.GetRotation();
  Serial.print(rotation.x);
  Serial.print("\t");
  Serial.print(rotation.y);
  Serial.print("\t");
  Serial.println(rotation.z);
}
