#include <PID.h>
#include <AccelGyro.h>

PID pid;
AccelGyro accelGyro;
Rotation rotation;
float output;

void setup() {
  Serial.begin(9600);
  accelGyro.Initialize();
  Serial.print("Is connected: ");
  Serial.println(accelGyro.IsConnected());
  if(!accelGyro.IsConnected()){
    Serial.println("Cannot get data from MPU6050");
    while(1);
  }
  pid.SetConstants(1.0, 0.005, 0.0);
  pid.SetDesiredValue(20.0);
  accelGyro.Calibrate();
}

void loop() {
  rotation = accelGyro.GetRotation();
  output = pid.Update(rotation.x);
  Serial.print(rotation.x);
  Serial.print(" ");
  Serial.println(output);
}
